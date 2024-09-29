/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if translation of program messages to the user's native
   language is requested. */
#define ENABLE_NLS 0

/* Define to 1 if you have the <adl_defines.h> header file. */
/* #undef HAVE_ADL_DEFINES_H */

/* Use AppIndicator3-0.1 */
#define HAVE_APPINDICATOR 1

/* Use libatasmart */
#define HAVE_ATASMART 1

/* Define to 1 if you have the MacOS X function CFLocaleCopyCurrent in the
   CoreFoundation framework. */
/* #undef HAVE_CFLOCALECOPYCURRENT */

/* Define to 1 if you have the MacOS X function CFPreferencesCopyAppValue in
   the CoreFoundation framework. */
/* #undef HAVE_CFPREFERENCESCOPYAPPVALUE */

/* Use CURL */
#define HAVE_CURL 1

/* Define if the GNU dcgettext() function is already present or preinstalled.
   */
#define HAVE_DCGETTEXT 1

/* Define if the GNU gettext() function is already present or preinstalled. */
#define HAVE_GETTEXT 1

/* Use GTOP */
#define HAVE_GTOP 1

/* Define if you have the iconv() function. */
/* #undef HAVE_ICONV */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Use JSON */
#define HAVE_JSON 1

/* Use JSON0 */
/* #undef HAVE_JSON_0 */

/* libatiadl is available */
/* #undef HAVE_LIBATIADL */

/* Use libmicrohttpd */
#define HAVE_LIBMICROHTTPD 1

/* Use desktop notification */
#define HAVE_LIBNOTIFY 1

/* Define to 1 if you have the `pthread' library (-lpthread). */
#define HAVE_LIBPTHREAD 1

/* Use libsensors */
#define HAVE_LIBSENSORS 1

/* Use libudisks */
/* #undef HAVE_LIBUDISKS2 */

/* Define to 1 if you have the <NVCtrl/NVCtrlLib.h> header file. */
/* #undef HAVE_NVCTRL_NVCTRLLIB_H */

/* Define to 1 if you have the <NVCtrl/NVCtrl.h> header file. */
/* #undef HAVE_NVCTRL_NVCTRL_H */

/* NVidia support enabled */
/* #undef HAVE_NVIDIA */

/* Remote monitoring enabled */
#define HAVE_REMOTE_SUPPORT 1

/* Define to 1 if you have the <sensors/error.h> header file. */
#define HAVE_SENSORS_ERROR_H 1

/* Define to 1 if you have the <sensors/sensors.h> header file. */
#define HAVE_SENSORS_SENSORS_H 1

/* Define to 1 if you have the <stdbool.h> header file. */
#define HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio.h> header file. */
#define HAVE_STDIO_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Use Unity */
/* #undef HAVE_UNITY */

/* Name of package */
#define PACKAGE "psensor"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "jeanfi@gmail.com"

/* Define to the full name of this package. */
#define PACKAGE_NAME "psensor"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "psensor 1.2.0"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "psensor"

/* Define to the home page for this package. */
#define PACKAGE_URL "http://wpitchoune.net/psensor"

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.2.0"

/* Define to 1 if all of the C90 standard headers exist (not just the ones
   required in a freestanding environment). This macro is provided for
   backward compatibility; new code need not use it. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "1.2.0"

/* Number of bits in a file offset, on hosts where this is settable. */
/* #undef _FILE_OFFSET_BITS */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */


#define PSENSOR_DESKTOP_FILE "psensor.desktop"
#define DATADIR "/usr/share/local/"
#define DEFAULT_WWW_DIR "/usr/share/psensor/www"
