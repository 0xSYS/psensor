{
    files = {
        "new-gui/UI/nk_defs.c"
    },
    depfiles = "nk_defs.o: new-gui/UI/nk_defs.c new-gui/ext_deps/nuklear/nuklear.h\
",
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