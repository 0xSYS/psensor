add_rules("mode.debug", "mode.release")



-- Library Dependencies
add_requires("json-c"       , {system = true})
add_requires("glib-2.0"     , {system = true})
add_requires("libgtop-2.0"  , {system = true})
add_requires("gio-unix-2.0" , {system = true})
add_requires("udisks2"      , {system = true})
add_requires("libatasmart"  , {system = true})
add_requires("X11"          , {system = true})

-- GUI Dependencies
add_requires("libxcb"        , {system = true})
add_requires("cairo"         , {system = true})
add_requires("xcb"           , {system = true})
add_requires("xcb-util"      , {system = true})
add_requires("xcb-keysyms"   , {system = true})
add_requires("xkbcommon"     , {system = true})
add_requires("xkbcommon-x11" , {system = true})
add_requires("freetype2"     , {system = true})

-- Server Dependencies
add_requires("libmicrohttpd" , {system = true})
add_requires("libgtop-2.0"   , {system = true})





















option("static-lib")
    set_default(true)
    set_showmenu(true)
    set_description("Build the psensor library as static")


option("shared-lib")
    set_default(false)
    set_showmenu(true)
    set_description("Build the psensor library as shared")




target("libpsensor")
    if has_config("shared-lib") then
        set_kind("shared")
    end
    
    if has_config("static-lib") then
        set_kind("static")
    end

    add_packages(
      "json-c",
      "glib-2.0",
      "libgtop-2.0",
      "gio-unix-2.0",
      "udisks2",
      "libatasmart",
      "X11"
    )

    add_includedirs(
      "lib/ext_lib/adl_sdk"
    )

    add_files("lib/*.c")


    target("psensor")
        set_default(false)
        set_kind("binary")


        add_includedirs(
            "lib/include",
            "new-gui/ext_deps/nuklear/backends",
            "new-gui/ext_deps/nuklear"
        )
        add_linkdirs("lib/build/linux/x64/release") -- Temporarry

        add_links("psensor")

        add_files(
          "mew-gui/ext_deps/cJSON/*.c",
          "new-gui/UI/*.c",
          "new-gui/*.c"
        )

        
