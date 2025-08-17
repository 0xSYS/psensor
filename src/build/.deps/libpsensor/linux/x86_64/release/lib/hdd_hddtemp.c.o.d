{
    depfiles_format = "gcc",
    files = {
        "lib/hdd_hddtemp.c"
    },
    depfiles = "hdd_hddtemp.o: lib/hdd_hddtemp.c lib/include/psensor/hdd.h  lib/include/psensor/bool.h lib/include/psensor/../../../config.h  lib/include/psensor/psensor.h lib/include/psensor/measure.h  lib/include/psensor/plog.h lib/include/psensor/psensor.h\
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