add_rules("mode.debug", "mode.release", "plugin.compile_commands.autoupdate")



-- Library Dependencies
add_requires("json-c"       , {system = true})
add_requires("glib-2.0"     , {system = true})
add_requires("libgtop-2.0"  , {system = true})
add_requires("gio-unix-2.0" , {system = true})
add_requires("udisks2"      , {system = true})
add_requires("libatasmart"  , {system = true})
add_requires("X11"          , {system = true})


-- New GUI Dependencies
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

-- Old GUI Deps
add_requires("gtk+-3.0"          , {system = true})
add_requires("x11"               , {system = true})
add_requires("libnotify"         , {system = true})
add_requires("libcurl"           , {system = true})
add_requires("unity"             , {system = true})
add_requires("appindicator3-0.1" , {system = true})
add_requires("pango"             , {system = true})
add_requires("cairo"             , {system = true})
add_requires("gdk-pixbuf-2.0"    , {system = true})
add_requires("atk"               , {system = true})
add_requires("freetype2"         , {system = true})
add_requires("harfbuzz"          , {system = true})
add_requires("glib-2.0"          , {system = true})
add_requires("atspi-2"           , {system = true})
add_requires("dbus-1"            , {system = true})
add_requires("fribidi"           , {system = true})
add_requires("libxml2"           , {system = true})
add_requires("pixman-1"          , {system = true})
add_requires("libpng16"          , {system = true})
add_requires("gio-unix-2.0"      , {system = true})
add_requires("blkid"             , {system = true})
add_requires("sysprof-capture-4" , {system = true})
add_requires("libgtop-2.0"       , {system = true})
add_requires("json-c"            , {system = true})
add_requires("p11-kit-1"         , {system = true})



option("static-lib")
    set_default(true)
    set_showmenu(true)
    set_description("Build the psensor library as static")


option("shared-lib")
    set_default(false)
    set_showmenu(true)
    set_description("Build the psensor library as shared")




target("psensor")
    if has_config("shared-lib") then
        set_kind("shared")
    end

    if has_config("static-lib") then
        set_kind("static")
    end

    --add_syslinks("udisks2", "XNVCtrl", "sensors")

    add_packages(
      "json-c",
      "glib-2.0",
      "libgtop-2.0",
      "gio-unix-2.0",
      "udisks2",
      "libatasmart",
      "X11"
    )

    add_syslinks("XNVCtrl", "sensors")



    add_includedirs(
      "lib/ext_lib/adl_sdk"
    )

    add_files("lib/ext_lib/CMessagePack/*.c", "lib/*.c")



-- GUI
target("psensor-ui")
    --set_default(false)
    set_kind("binary")


    add_includedirs(
        "lib/include",
        "new-gui/ext_deps/nuklear/backends",
        "new-gui/ext_deps/nuklear",
        "new-gui/ext_deps",
        "new-gui/ext_deps/cJSON"
    )
    add_linkdirs("build/linux/x86_64/release") -- Temporarry
    add_packages(
        "libxcb",
        "cairo",
        "xcb",
        "xcb-util",
        "xcb-keysyms",
        "xkbcommon",
        "xkbcommon-x11",
        "freetype2"
    )
    add_deps("psensor")

    add_defines("LOGC__USER_SETTINGS", "CJSON_HIDE_SYMBOLS")

    add_files(
      "new-gui/ext_deps/cJSON/*.c",
      "new-gui/ext_deps/log_c/*.c",
      "new-gui/*.c",
      "new-gui/UI/*.c"
    )


    after_build(function (target)
        -- Copy the assets and the css theme next to the build directory
        os.cp("../assets/Liter-Regular.ttf", target:targetdir())
    end)



target("psensor-old")
    set_kind("binary")
    add_includedirs(
        "lib/include",
        "server",
        "GUI"
    )
    add_cxflags("-fvisibility=default", {force = true})
    add_ldflags("-rdynamic", "-Wl,--as-needed", {force = true})
    add_defines(
        "GTK_DISABLE_SINGLE_INCLUDES",
        "GDK_DISABLE_DEPRECATED",
        "WITH_GZFILEOP",
        "GSEAL_ENABLE"
    )
    add_linkdirs("build/linux/x86_64/release")
    add_deps("psensor") -- Didn't know I had to use add_deps instead ;(
    add_syslinks("pthread", "m")
    add_packages(
        "gtk+-3.0"         ,
        "x11"              ,
        "libnotify"        ,
        "libcurl"          ,
        "unity"            ,
        "appindicator3-0.1",
        "pango"            ,
        "cairo"            ,
        "gdk-pixbuf-2.0"   ,
        "atk"              ,
        "freetype2"        ,
        "harfbuzz"         ,
        "glib-2.0"         ,
        "atspi-2"          ,
        "dbus-1"           ,
        "fribidi"          ,
        "libxml2"          ,
        "pixman-1"         ,
        "libpng16"         ,
        "gio-unix-2.0"     ,
        "blkid"            ,
        "sysprof-capture-4",
        "libgtop-2.0"      ,
        "json-c"           ,
        "p11-kit-1"
    )

    add_files(
      "GUI/*.c"
    )