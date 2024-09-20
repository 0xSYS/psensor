project('LibPSensor', 'c')
deps = [ dependency('json-c'), dependency('glib-2.0'), dependency('libgtop-2.0'), dependency('gio-unix-2.0'), dependency('udisks2')]
sources = [ 'amd.c', 'color.c', 'hdd_atasmart.c', 'hdd_hddtemp.c', 'lmsensor.c', 'measure.c', 'nvidia.c', 'pgtop2.c', 'pio.c', 'plog.c', 'pmutex.c', 'psensor.c', 'ptime.c', 'pudisks2.c', 'slog.c', 'temperature.c', 'url.c']
static_library('libpsensor', sources, dependencies : deps)
