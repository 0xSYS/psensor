project('PSensor Web Server', 'c')



psensorLibDeps = \
[ \
  dependency('json-c'), \
  dependency('glib-2.0'), \
  dependency('libgtop-2.0'), \
  dependency('gio-unix-2.0'), \
  dependency('udisks2'), \
  dependency('libatasmart') \
]

psensorLibSrc = \
[ \
  '../lib/amd.c', \
  '../lib/psensor_json.c', \
  '../lib/color.c', \
  '../lib/hdd_atasmart.c', \
  '../lib/hdd_hddtemp.c', \
  '../lib/lmsensor.c', \
  '../lib/measure.c', \
  '../lib/nvidia.c', \
  '../lib/pgtop2.c', \
  '../lib/pio.c', \
  '../lib/plog.c', \
  '../lib/pmutex.c', \
  '../lib/psensor.c', \
  '../lib/ptime.c', \
  '../lib/pudisks2.c', \
  '../lib/slog.c', \
  '../lib/temperature.c', \
  '../lib/url.c'\
]

psensorLib = static_library('psensor', psensorLibSrc, dependencies : psensorLibDeps)

psensorInclude = [ include_directories('../lib/include/'), include_directories('../') ]


psensorWebServerDeps = \
[ \
  dependency('json-c'), \
  dependency('libmicrohttpd'), \
  dependency('libgtop-2.0'), \
]

psensorWebServerSrc = \
[ \
  'server.c', \
  'sysinfo.c' \
]

executable('psensor-server', \
psensorWebServerSrc, \
link_with : psensorLib, \
include_directories : psensorInclude, \
dependencies : psensorWebServerDeps,
link_args : ['-lsensors'])
