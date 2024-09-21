# PSensor Library
### The core of the psensor utilities

The library contains functions used for reading sensor values, scaning sysfs directories for cooling fans and controlling the fans.

# API

- [amd.h](#amd.h)
- [color.h](#color.h)
- [hdd.h](#hdd.h)
- [lmsensor.h](#lmsensor.h)
- [measure.h](#measure.h)
- [nvidia.h](#nvidia.h)
- [pgtop.h](#pgtop.h)
- [pio.h](#pio.h)
- [plog.h](#plog.h)
- [pmutex.h](#pmutex.h)
- [psensor.h](#psensor.h)
- [psensor_json.h](#psensor_json.h)
- [ptime.h](#ptime.h)
- [pudisks2.h](#pudisks2.h)
- [slog.h](#slog.h)
- [temperature.h](#temperature.h)
- [url.h](#url.h)

# How to build

## installation requirements

Fedora:
`sudo dnf install meson ninja-build`

## Dependencies
```
sudo dnf install json-c-devel glib2-devel libgtop2-devel libudisks2-devel libatasmart-devel libXNVCtrl-devel
```

## Configuring build
By default the library build is static but you can also build shared library (build_type: shared, static)

In the source directory:
```
cd lib
meson setup builddir
```

or if you need shared library:
`meson setup builddir -Dbuild_type=shared`

## Building the library
`meson compile -C builddir`

<a id="amd.h"></a>
### amd.h
```c
static inline bool amd_is_supported(void) { return false; }
static inline void amd_psensor_list_update(struct psensor **s) {}
static inline void amd_psensor_list_append(struct psensor ***s, int n) {}
static inline void amd_cleanup(void) {}
```

<a id="color.h"></a>
### color.h
```c
/* Represents a RGB color. */
struct color {
	/* rgb floating 0..1 */
	double red;
	double green;
	double blue;
};

/** rgb 0..1 */
struct color *color_new(double r, double g, double b);
struct color *color_dup(struct color *);

/** rgb 0..1 */
void color_set(struct color *, double r, double g, double b);

int is_color(const char *str);
struct color *str_to_color(const char *str);
char *color_to_str(const struct color *color);
```

<a id="hdd.h"></a>
### hdd.h
```c
void atasmart_psensor_list_append(struct psensor ***, int);
void atasmart_psensor_list_update(struct psensor **);
void hddtemp_psensor_list_append(struct psensor ***sensors, int values_length);
void hddtemp_psensor_list_update(struct psensor **sensors);
```

<a id="lmsensor.h"></a>
### lmsensor.h
```c
static inline bool lmsensor_is_supported(void) { return true; }
void lmsensor_psensor_list_update(struct psensor **);
void lmsensor_psensor_list_append(struct psensor ***, int);
void lmsensor_cleanup(void);
```

<a id="measure.h"></a>
### measure.h
```c
#define UNKNOWN_DBL_VALUE DBL_MIN

struct measure {
	double value;
	struct timeval time;
};

void measure_copy(struct measure *src, struct measure *dst);
struct measure *measures_dbl_create(int size);
void measures_free(struct measure *measures);
```

<a id="nvidia.h"></a>
### nvidia.h
```c
static bool nvidia_is_supported(void) { return true; }
void nvidia_psensor_list_update(struct psensor **);
void nvidia_psensor_list_append(struct psensor ***, int);
void nvidia_cleanup(void);
```

<a id="pgtop2.h"></a>
### pgtop2.h
```c
static inline bool gtop2_is_supported(void) { return true; }
struct psensor *create_cpu_usage_sensor(int);
void cpu_usage_sensor_update(struct psensor *);
void gtop2_psensor_list_update(struct psensor **);
void gtop2_psensor_list_append(struct psensor ***, int);
```

<a ="pio.h"></a>
### pio.h
```c
#define P_IO_VER 6

/* Returns '1' if a given 'path' denotates a directory else returns
 * 0
 */
int is_dir(const char *path);

/* Returns '1' if a given 'path' denotates a file else returns
 * 0
 */
int is_file(const char *path);

/* Returns a normalized path */
char *path_normalize(const char *dpath);

/* Returns the null-terminated entries of a directory */
char **dir_list(const char *dpath, int (*filter) (const char *path));
void paths_free(char **paths);

char *path_append(const char *dir, const char *path);

/*
 * Returns the size of a file.
 * Returns '-1' if the size cannot be retrieved or not a file.
 */
long file_get_size(const char *path);

/*
 * Returns the content of a file.
 * Returns 'NULL' if the file cannot be read or failed to allocate
 * enough memory.
 * Returns an empty string if the file exists but is empty.
 */
char *file_get_content(const char *path);

enum file_copy_error {
	FILE_COPY_ERROR_OPEN_SRC = 1,
	FILE_COPY_ERROR_OPEN_DST,
	FILE_COPY_ERROR_READ,
	FILE_COPY_ERROR_WRITE,
	FILE_COPY_ERROR_ALLOC_BUFFER
};

void file_copy_print_error(int code, const char *src, const char *dst);

/*
 * Copy a file.
 *
 * Returns '0' if sucessfull, otherwise return the error code.
 */
int file_copy(const char *src, const char *dst);

int dir_rcopy(const char *, const char *);

void mkdirs(const char *dirs, mode_t mode);
```

<a id="plog.h"></a>
### plog.h
```c
#define P_LOG_VER 3

enum log_level {
	LOG_ERR,
	LOG_WARN,
	LOG_INFO,
	LOG_DEBUG
};

void log_open(const char *path);
void log_close(void);

void log_printf(int lvl, const char *fmt, ...);
void log_debug(const char *fmt, ...);
void log_err(const char *fmt, ...);
void log_info(const char *fmt, ...);
void log_warn(const char *fmt, ...);

void _log(const char *fct, const char *fmt, ...);

#define log_fct(...) _log(__func__, __VA_ARGS__)
#define log_fct_enter() log_fct("ENTER")
#define log_fct_exit() log_fct("EXIT")

/* level of the log file. */
extern int log_level;
```

<a id="pmutex.h"></a>
### pmutex.h
```c
int pmutex_lock(pthread_mutex_t *);
int pmutex_unlock(pthread_mutex_t *);
int pmutex_init(pthread_mutex_t *);
```

<a id="psensor.h"></a>
### psensor.h
```c
#define MAX_HWMON_DIRS 9
#define SMALL_BUF_SIZE 128

enum psensor_type {
	/* type of sensor values */
	SENSOR_TYPE_TEMP = 0x00001,
	SENSOR_TYPE_RPM = 0x00002,
	SENSOR_TYPE_PERCENT = 0x00004,

	/* Whether the sensor is remote */
	SENSOR_TYPE_REMOTE = 0x00008,

	/* Libraries used for retrieving sensor information */
	SENSOR_TYPE_LMSENSOR = 0x00100,
	SENSOR_TYPE_NVCTRL = 0x00200,
	SENSOR_TYPE_GTOP = 0x00400,
	SENSOR_TYPE_ATIADL = 0x00800,
	SENSOR_TYPE_ATASMART = 0x01000,
	SENSOR_TYPE_HDDTEMP = 0x02000,
	SENSOR_TYPE_UDISKS2 = 0x800000,

	/* Type of HW component */
	SENSOR_TYPE_HDD = 0x04000,
	SENSOR_TYPE_CPU = 0x08000,
	SENSOR_TYPE_GPU = 0x10000,
	SENSOR_TYPE_FAN = 0x20000,

	SENSOR_TYPE_GRAPHICS = 0x40000,
	SENSOR_TYPE_VIDEO = 0x80000,
	SENSOR_TYPE_PCIE = 0x100000,
	SENSOR_TYPE_MEMORY = 0x200000,
	SENSOR_TYPE_AMBIENT = 0x400000,

	/* Combinations */
	SENSOR_TYPE_HDD_TEMP = (SENSOR_TYPE_HDD | SENSOR_TYPE_TEMP),
	SENSOR_TYPE_CPU_USAGE = (SENSOR_TYPE_CPU | SENSOR_TYPE_PERCENT)
};

typedef struct
{
  char **pwmFiles;
  char **pwmEnableFiles;
  int fanInputCount;  // Number of fanX_input files
} psensor_fan;


struct psensor {
	/* Human readable name of the sensor.  It may not be uniq. */
	char *name;

	/* Uniq id of the sensor */
	char *id;

	/* Name of the chip. */
	char *chip;

	/* Maximum length of 'values' */
	int values_max_length;

	/*
	 * Last registered measures of the sensor.  Index 0 for the
	 * oldest measure.
	 */
	struct measure *measures;

	/* see psensor_type */
	unsigned int type;

	double max;

	double min;

	/* The highest value detected during this session. */
	double sess_highest;

	/* The lowest value detected during this session. */
	double sess_lowest;

	double alarm_high_threshold;
	double alarm_low_threshold;

	/* Whether an alarm is raised for this sensor */
	bool alarm_raised;

	void (*cb_alarm_raised)(struct psensor *, void *);
	void *cb_alarm_raised_data;

//#ifdef HAVE_LIBATIADL
	/* AMD id for the aticonfig */
	int amd_id;
//#endif

	void *provider_data;
	void (*provider_data_free_fct)(void *);
};

struct psensor *psensor_create(char *id,
			       char *name,
			       char *chip,
			       unsigned int type,
			       int values_max_length);

void psensor_values_resize(struct psensor *s, int new_size);

void psensor_free(struct psensor *sensor);

void psensor_list_free(struct psensor **sensors);
int psensor_list_size(struct psensor **sensors);

struct psensor *psensor_list_get_by_id(struct psensor **sensors,
				       const char *id);

int is_temp_type(unsigned int type);

double get_min_temp(struct psensor **sensors);
double get_max_temp(struct psensor **sensors);

double get_min_rpm(struct psensor **sensors);
double get_max_rpm(struct psensor **sensors);

/*
 * Converts the value of a sensor to a string.
 *
 * parameter 'type' is SENSOR_TYPE_LMSENSOR_TEMP, SENSOR_TYPE_NVIDIA,
 * or SENSOR_TYPE_LMSENSOR_FAN
 */
char *psensor_value_to_str(unsigned int type,
			   double value,
			   int use_celsius);

char *psensor_measure_to_str(const struct measure *m,
			     unsigned int type,
			     unsigned int use_celsius);

struct psensor **psensor_list_add(struct psensor **sensors,
				  struct psensor *sensor);

void psensor_list_append(struct psensor ***sensors, struct psensor *sensor);

struct psensor **psensor_list_copy(struct psensor **);

void psensor_set_current_value(struct psensor *sensor, double value);
void psensor_set_current_measure(struct psensor *sensor, double value,
				 struct timeval tv);

double psensor_get_current_value(const struct psensor *);

struct measure *psensor_get_current_measure(struct psensor *sensor);

/* Returns a string representation of a psensor type. */
const char *psensor_type_to_str(unsigned int type);

const char *psensor_type_to_unit_str(unsigned int type, int use_celsius);

double get_max_value(struct psensor **sensors, int type);

char *psensor_current_value_to_str(const struct psensor *, unsigned int);

void psensor_log_measures(struct psensor **sensors);


/*
Detect existing cooling fans addressed by the linux kernel inside hwmon directory
Returns a structure composed out of these parameters:
- fan.._input file count which represents a phisycal fan inside your computer.
- pwm_enable files which allow the fans to be controlled by writing a PWM value
- pwm files. Those files hold the PWM value assigned to them
- NULL if no fans could be detected
*/
psensor_fan *psensor_detectFans();

/*
Enable PWM fan control
Requires full path to pwm enable file and a value, usually 1 or 0
Returns 0 if the function is executed successfully if not it returns 1
Example: int fn_stat = psensor_enable_fan_pwm("/sys/class/hwmon/hwmon4/pwm1_enable", 1);
*/
int psensor_enable_fan_pwm(const char *hwmnoDirPath, int v);

/*
Write a PWM value to the existing fan pwm files created by the kernel.
Requires full path to the hardware directory + a PWM value (0 - 255 only!)
Returns 0 if the function is executed successfully if not it returns 1
Example: int fn_stat = psensor_fan_set_pwm("/sys/class/hwmon/hwmon4/pwm1", 230);
*/
int psensor_fan_set_pwm(const char * hwClassDir, int PWM);

/*
Get the last pwm value from hwmon sysfs
Requires full path to pwm file
Returns 0 by default and if it fails, if not it returns the last value of a PWM file
Example int LastFanPwm = psensor_get_last_pwm("/sys/class/hwmon/hwmon4/pwm1");
*/
int psensor_get_last_pwm(const char * path);


/*
Test the speed of a fan by slowly increasing and decreasing the PWM value
Requires the full path to the PWM file
Returns 0 if the function is executed successfully if not it returns 1
Example: int fn_stat = psensor_test_fan("/sys/class/hwmon/hwmon3/pwm1");
*/
int psensor_test_fan(const char * hwmonDir);
```

<a id="psensor_json.h"></a>
### psensor_json.h
```c
char *sensor_to_json_string(struct psensor *s);
char *sensors_to_json_string(struct psensor **sensors);

/*
 * Creates a new allocated psensor corresponding to a given json
 * representation.
 */
struct psensor *psensor_new_from_json(json_object *o, const char *sensors_url, int values_max_length);
```

<a id="ptime.h"></a>
### ptime.h
```c
extern const int P_TIME_VER;

char *get_current_ISO8601_time();

char *time_to_ISO8601_time(time_t *);
char *time_to_ISO8601_date(time_t *);

char *tm_to_ISO8601_date(struct tm *);
char *tm_to_ISO8601_time(struct tm *);
```

<a id="pudisks2.h"></a>
### pudisks2.h
```c
static bool udisks2_is_supported(void) { return true; }

void udisks2_psensor_list_append(struct psensor ***, int);
void udisks2_psensor_list_update(struct psensor **);
```

<a id="slog.h"></a>
### slog.h
```c
bool slog_activate(const char *, struct psensor **, pthread_mutex_t *, int s);
void slog_close(void);
```

<a id="temperature.h"></a>
### temperature.h
```c
double celsius_to_fahrenheit(double c);
double fahrenheit_to_celsius(double c);
double kelvin_to_celsius(double k);
```

<a id="url.h"></a>
### url.h
```c
char *url_encode(const char *str);
char *url_normalize(const char *url);
```
