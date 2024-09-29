project('LibPSensor', 'c', default_options: ['build_type=static'])

deps = \
[ \
  dependency('json-c'), \
  dependency('glib-2.0'), \
  dependency('libgtop-2.0'), \
  dependency('gio-unix-2.0'), \
  dependency('udisks2'), \
  dependency('libatasmart'), \
  dependency('X11') \
]

sources = \
[ \
  'amd.c', \
  'color.c', \
  'hdd_atasmart.c', \
  'hdd_hddtemp.c', \
  'lmsensor.c', \
  'measure.c', \
  'nvidia.c', \
  'pgtop2.c',\
  'plog.c', \
  'pio.c', \
  'pmutex.c', \
  'psensor.c', \
  'ptime.c', \
  'pudisks2.c', \
  'slog.c', \
  'temperature.c', \
  'url.c', \
  'precord.c', \
  'ext_lib/CMessagePack/cmp.c' \
]

buildType = get_option('build_type')

#By default the build of the core library is static
if buildType == 'static'
  static_library('psensor', sources, dependencies : deps, link_args : ['-lXNVCtrl', '-lsensors'])
elif buildType == 'shared'
  shared_library('psensor', sources, dependencies : deps, link_args : ['-lXNVCtrl', '-lsensors'])
else
  error('Unknown build type!')
endif
