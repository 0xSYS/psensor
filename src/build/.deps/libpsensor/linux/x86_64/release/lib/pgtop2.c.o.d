{
    files = {
        "lib/pgtop2.c"
    },
    depfiles_format = "gcc",
    depfiles = "pgtop2.o: lib/pgtop2.c lib/include/psensor/pgtop2.h  lib/include/psensor/bool.h lib/include/psensor/../../../config.h  lib/include/psensor/psensor.h lib/include/psensor/measure.h  lib/include/psensor/plog.h\
",
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-fvisibility=hidden",
            "-O3",
            "-Ilib/ext_lib/adl_sdk",
            "-isystem",
            "/usr/include/json-c",
            "-isystem",
            "/usr/include/libgtop-2.0",
            "-isystem",
            "/usr/include/gio-unix-2.0",
            "-isystem",
            "/usr/include/udisks2",
            "-isystem",
            "/usr/include/libmount",
            "-isystem",
            "/usr/include/blkid",
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