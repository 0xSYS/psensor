{
    depfiles = "ui_main.o: new-gui/UI/ui_main.c new-gui/UI/ui_main.h  new-gui/ext_deps/nuklear/nuklear.h  new-gui/ext_deps/nuklear/backends/nuklear_xcb.h new-gui/UI/style.h  new-gui/UI/main_window.h\
",
    files = {
        "new-gui/UI/ui_main.c"
    },
    depfiles_format = "gcc",
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-fvisibility=hidden",
            "-O3",
            "-Ilib/include",
            "-Inew-gui/ext_deps/nuklear/backends",
            "-Inew-gui/ext_deps/nuklear",
            "-isystem",
            "/usr/include/cairo",
            "-isystem",
            "/usr/include/pixman-1",
            "-isystem",
            "/usr/include/freetype2",
            "-isystem",
            "/usr/include/libpng16",
            "-isystem",
            "/usr/include/harfbuzz",
            "-isystem",
            "/usr/include/glib-2.0",
            "-isystem",
            "/usr/lib/glib-2.0/include",
            "-isystem",
            "/usr/include/sysprof-6",
            "-pthread",
            "-DNDEBUG"
        }
    }
}