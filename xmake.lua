add_rules("mode.debug", "mode.release")

set_policy("build.warning", true)
set_warnings("all", "extra")

add_syslinks("ssl", "crypto")
target("sa")
    set_kind("shared")
    add_files("SA/**/*.c")
    if is_mode("debug") then
        add_defines("DEBUG")
        set_targetdir("./bin/debug")
    else
        set_targetdir("./bin/release")
    end

target("tests")
    set_kind("binary")
    add_files("tests/tests.c")
    if is_mode("debug") then
        add_defines("DEBUG")
        set_targetdir("./bin/debug")
    else
        set_targetdir("./bin/release")
    end
    add_cxflags("-O3", "-pedantic")
    add_deps("sa")