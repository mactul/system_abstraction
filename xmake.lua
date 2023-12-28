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
    add_requires("SDL2", {optional = true})
    add_requires("gdi32", {system = true, optional = true})
    add_requires("xcb", {system = true, optional = true})
    add_requires("X11", {system = true, optional = true})
end

option("build_network")
    set_default(true)
    set_showmenu(true)

option("build_tests")
    set_default(false)
    set_showmenu(true)

option("build_graphics")
    on_check(
        function (truc) 
            print("bonjour")
        end
    )
    set_default(true)
    set_showmenu(true)

option("graphics_renderer")
    set_default("autoconfig")
    set_showmenu(true)
    set_values("autoconfig", "windows_gdi", "sdl2", "x11", "xcb")


function configure(target)
    function link_graphics(target)
        import("lib.detect.find_library")

        if get_config("graphics_renderer") == "autoconfig" then
            if find_library("SDL2", {"/usr/lib", "/usr/local/lib", "./"}) then
                graphics_renderer = "sdl2"
            elseif is_plat("windows", "mingw") then
                graphics_renderer = "windows_gdi"
            else
                graphics_renderer = "x11"
            end
        else
            graphics_renderer = get_config("graphics_renderer")
        end

        if graphics_renderer == "sdl2" then
            target:add("links", "SDL2")
            target:add("defines", "SA_GRAPHICS_SDL2")
        else
            target:remove("files", "SA/graphics/SDL2/*.c")
        end

        if graphics_renderer == "windows_gdi" then
            target:add("links", "gdi32")
            target:add("defines", "SA_GRAPHICS_WINDOWS_GDI")
        else
            target:remove("files", "SA/graphics/windows_gdi/*.c")
        end

        if graphics_renderer == "x11" then
            target:add("links", "X11")
            target:add("defines", "SA_GRAPHICS_X11")
        else
            target:remove("files", "SA/graphics/x11/*.c")
        end

        if graphics_renderer == "xcb" then
            target:add("links", "xcb")
            target:add("defines", "SA_GRAPHICS_XCB")
        else
            target:remove("files", "SA/graphics/xcb/*.c")
        end
    end


    if is_mode("debug") then
        target:add("defines", "DEBUG")
    end

    if has_config("build_graphics") then
        target:add("links", "pthread")
        target:add("defines", "Linux")
        target:add("defines", "_REENTRANT")

        link_graphics(target)
    else
        target:add("defines", "SA_GRAPHICS_DISABLED")
        target:remove("files", "SA/graphics/**")
    end
end

rule("fine_config")
    on_config(configure)


target("sa")
    set_kind("shared")
    add_includedirs("./", {public = true})
    add_headerfiles("(SA/**.h)")
    add_files("SA/**/*.c")
    add_options("build_network")
    add_options("build_graphics")

    add_rules("fine_config")

    if has_config("build_network") then
        add_files("SA/network/*.c")
        add_packages("openssl")
    else
        add_defines("SA_NETWORK_DISABLED")
        remove_files("SA/network/*")
    end

if has_config("build_tests") then
    target("tests")
        add_files("tests/*.c")
        add_defines("DEBUG")
        add_deps("sa")
end