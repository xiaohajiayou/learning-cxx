add_rules("mode.debug", "mode.release")
set_encodings("utf-8")
set_warnings("all")
set_languages("cxx17")

target("test")
    set_kind("static")
    add_defines(string.format("__XMAKE__=\"%s\"", os.scriptdir():gsub("\\", "/")))
    add_files("learn/test.cpp")

target("learn")
    set_kind("binary")
    add_deps("test")
    add_files("learn/learn.cpp")

target("summary")
    set_kind("binary")
    add_deps("test")
    add_files("learn/summary.cpp")
    if is_plat("linux", "macosx", "bsd") then
        add_syslinks("pthread")
    end
    add_defines(string.format("__XMAKE__=\"%s\"", os.scriptdir():gsub("\\", "/")))
