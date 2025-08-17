{
    depfiles_format = "gcc",
    files = {
        "lib/slog.c"
    },
    depfiles = "slog.o: lib/slog.c lib/../config.h lib/include/psensor/bool.h  lib/include/psensor/../../../config.h lib/include/psensor/plog.h  lib/include/psensor/pmutex.h lib/include/psensor/ptime.h  lib/include/psensor/slog.h lib/include/psensor/psensor.h  lib/include/psensor/bool.h lib/include/psensor/measure.h  lib/include/psensor/plog.h\
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