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
    if get_config("graphics_renderer") == "autoconfig" then 
        if is_plat("windows", "mingw") then
            graphics_renderer = "windows_gdi"
        else
            graphics_renderer = "xcb"
        end
    else
        graphics_renderer = get_config("graphics_renderer")
    end
    
    if graphics_renderer == "sdl2" then
        add_requires("sdl2")
    elseif graphics_renderer == "windows_gdi" then
        add_requires("gdi32", {system = true})
    elseif graphics_renderer == "xcb" then
        add_requires("xcb", {system = true})
    elseif graphics_renderer == "X11" then
        add_requires("X11", {system = true})
    else
        print("error: unknown graphics renderer")
        graphics_renderer(1)
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

option("graphics_renderer")
    set_default("autoconfig")
    set_showmenu(true)
    set_values("autoconfig", "windows_gdi", "sdl2", "x11", "xcb")

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
        add_defines("_REENTRANT")

        if graphics_renderer == "xcb" then
            add_links("xcb");
            add_files("SA/graphics/xcb/*.c")
            add_defines("SA_GRAPHICS_xcb")
        else
            remove_files("SA/graphics/xcb/*.c")
        end

        if graphics_renderer == "windows_gdi" then
            add_links("gdi32");
            add_files("SA/graphics/windows_gdi/*.c")
            add_defines("SA_GRAPHICS_WINDOWS_GDI")
        else
            remove_files("SA/graphics/windows_gdi/*.c")
        end

        if graphics_renderer == "sdl2" then
            add_files("SA/graphics/SDL2/*.c")
            add_packages("sdl2")
            add_defines("SA_GRAPHICS_SDL2")
        else
            remove_files("SA/graphics/SDL2/*.c")
        end

        if graphics_renderer == "x11" then
            add_links("X11");
            add_files("SA/graphics/x11/*.c")
            add_defines("SA_GRAPHICS_X11")
        else
            remove_files("SA/graphics/x11/*.c")
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