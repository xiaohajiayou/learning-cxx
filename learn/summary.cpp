#include "test.h"
#include <atomic>
#include <chrono>
#include <cstring>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <thread>
#include <vector>

#ifndef __XMAKE__
#define __XMAKE__ "XMAKE is not defined"
#endif

constexpr auto MAX_EXERCISE = 33;
constexpr auto PROJECT_ROOT = __XMAKE__;
constexpr auto XMAKE_FALLBACK = "XMAKE is not defined";

int main(int argc, char **argv) {
    namespace fs = std::filesystem;
    std::error_code ec;
    const fs::path project_root{PROJECT_ROOT};
    const bool has_valid_root = std::strcmp(PROJECT_ROOT, XMAKE_FALLBACK) != 0 && fs::exists(project_root);
    if (has_valid_root) {
        fs::current_path(project_root, ec);
        if (ec) {
            std::cerr << "Failed to switch current directory to " << project_root << ": " << ec.message() << std::endl;
            return EXIT_FAILURE;
        }
    }

    if (argc == 1) {
        Log log{Console{}};
        for (auto i = 0; i <= MAX_EXERCISE; ++i) {
            log << i;
        }
        std::cout << std::accumulate(log.result.begin(), log.result.end(), 0, std::plus{}) << '/' << MAX_EXERCISE + 1 << " [";
        for (auto b : log.result) {
            std::cout << (b ? "\x1b[32m#\x1b[0m" : "\x1b[31mX\x1b[0m");
        }
        std::cout << ']' << std::endl;
        return EXIT_SUCCESS;
    }
    if (argc == 2 && std::strcmp(argv[1], "--simple") == 0) {
        auto concurrency = std::thread::hardware_concurrency();
        if (concurrency == 0) {
            concurrency = 1;
        }

        std::atomic_int k{0};
        std::vector<std::thread> threads;
        threads.reserve(concurrency);

        std::cout << "concurrency: " << concurrency << std::endl;
        Log log{Null{}};
        for (auto i = 0u; i <= concurrency; ++i) {
            threads.emplace_back([i, &log, &k] {
                int j = k.fetch_add(1);
                while (j <= MAX_EXERCISE) {
                    std::printf("run %d at %d\n", j, i);
                    log << j;
                    j = k.fetch_add(1);
                }
            });
        }
        for (auto &thread : threads) {
            thread.join();
        }

        std::cout << std::accumulate(log.result.begin(), log.result.end(), 0, std::plus{}) << '/' << MAX_EXERCISE + 1 << std::endl;
        return EXIT_SUCCESS;
    }
    std::cerr << "Usage: xmake run summary [--simple]" << std::endl;
    return EXIT_FAILURE;
}
