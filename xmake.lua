add_rules("mode.debug", "mode.release")

set_policy("build.warning", true)
set_warnings("all", "extra")

if is_plat("windows", "mingw") then 
    if has_config("build_network") then
        add_requires("openssl")
    end
else 
    if has_config("build_network") then
        add_requires("openssl3", { alias = "openssl" })
    end
end 

option("build_network")
    set_default(true)
    set_showmenu(true)

option("build_tests")
    set_default(false)
    set_showmenu(true)

rule("flags_extras")
    if is_mode("debug") then
        on_config(function(target)
            target:add("defines", "DEBUG")
        end)
    end

target("sa")
    set_kind("shared")
    add_includedirs("./", {public = true})
    add_headerfiles("(SA/**.h)")
    add_files("SA/**/*.c")
    add_rules("flags_extras")
    add_options("build_network")
    if has_config("build_network") then
        add_files("SA/network/*.c")
        add_headerfiles("(SA/**.h)")
        add_packages("openssl")
    else
        add_defines("SA_NETWORK_DISABLED")
        remove_files("SA/network/*")
    end

if has_config("build_tests") then
    target("tests")
        add_files("tests/tests.c")
        add_rules("flags_extras")
        add_cxflags("-O3", "-pedantic")
        add_deps("sa")
end