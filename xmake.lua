add_rules("mode.debug", "mode.release")

set_policy("build.warning", true)
set_warnings("all", "extra")

if has_config("build_network") then
    if is_plat("windows", "mingw") then
        add_requires("openssl")
    else
        add_requires("openssl3", { alias = "openssl" })
    end
end

if has_config("build_graphics") then
    if has_config("graphics_sdl2") then
        add_requires("sdl2")
    end
end

option("build_network")
    set_default(true)
    set_showmenu(true)

option("build_tests")
    set_default(false)
    set_showmenu(true)

option("build_graphics")
    set_default(true)
    set_showmenu(true)

option("graphics_sdl2")
    set_default(true)
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
    add_options("build_graphics")
    if has_config("build_network") then
        add_files("SA/network/*.c")
        add_packages("openssl")
    else
        add_defines("SA_NETWORK_DISABLED")
        remove_files("SA/network/*")
    end

    if has_config("build_graphics") then
        add_links("pthread")
        add_defines("Linux")
        if has_config("graphics_sdl2") then
            add_files("SA/graphics/SDL2/*.c")
            add_packages("sdl2")
            add_defines("SA_GRAPHICS_SDL2")
        end
    else
        add_defines("SA_GRAPHICS_DISABLED")
        remove_files("SA/graphics/**")
    end

if has_config("build_tests") then
    target("tests")
        add_files("tests/*.c")
        add_rules("flags_extras")
        add_deps("sa")
end