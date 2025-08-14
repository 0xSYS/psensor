{
    files = {
        "lib/precord.c"
    },
    depfiles = "precord.o: lib/precord.c lib/include/psensor/precord.h  lib/include/psensor/plog.h lib/ext_lib/CMessagePack/cmp.h\
",
    depfiles_format = "gcc",
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