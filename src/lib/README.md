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
