# PSensor

Psensor is a graphical hardware monitoring application for Linux.


### It draws realtime charts and raises alerts about:
* the temperature of the motherboard and CPU sensors (using lm-sensors).
* the temperature of the NVidia GPUs (using XNVCtrl).
* the temperature of ATI GPUs (using ATI ADL SDK).
* the temperature of the Hard Disk Drives (using hddtemp, libatasmart or udisks2).
* the rotation speed of the fans.
* the temperature of a remote computer.
* the CPU load.

### Alerts are using Desktop Notification and a specific GTK+ status icon.

> [!NOTE]
> For Ubuntu users, Psensor is providing an Application Indicator which turns red under alert and a menu for quickly check all sensors.
>
> A new feature of PSensor is to allow manual fan speed controlling by writing PWM values to the pwm controllers
> each asociated to the certain fan managed by the kernel.
> ### This only works with 4 pin based cooling fans!!!
>
> It relies on a simple user interface that lists all available cooling fans inside your computer.
> (A UI redesign to the fan controller will be made soon).

### PSensor is composed out of 3 important components:
- [Core library](src/lib/README.md)
- [GUI Utility](src/GUI/README.md)
- [Web Server](src/server/README.md)

# Building fromm source

> [!WARNING]
> The GUI utility will throw an error about missing gschema settings !!!
> This happens On ceratain distros like debian / ubuntu and some other ones.

## Installation requirements:

### Meson build
`sudo dnf install meson ninja-build`

### Dependencies (For all 3 components)
```
sudo dnf install gtk3-devel \
libnotify-devel \
libcurl-devel \
libunity-devel \
libappindicator-devel \
pango-devel \
cairo-devel \
gdk-pixbuf2-devel \
atk-devel \
ghc-gi-freetype2-devel \
harfbuzz-devel \
glib-devel \
dbus-devel \
fribidi-devel \
libxml2-devel \
pixman-devel \
libpng-devel \
libblkid-devel \
sysprof-capture-devel \
p11-kit-devel \
lm_sensors-devel \
json-c-devel \
glib2-devel \
libgtop2-devel \
libudisks2-devel \
libatasmart-devel \
```

## Step 2

```
git clone https://github.com/0xSYS/psensor/tree/meson-build-syst?tab=readme-ov-file
```

Buldding the core library first is an optional step as I added the even the sources to the library.
So the library and the utilities (GUI and web server) will build at the same time

## Building GUI utility
```
cd src/GUI
meson setup builddir
meson compile -C build dir
```

## Building the web server utility
```
cd src/server
meson setup builddir
meson compile -C builddir
```

> [!NOTE]
> Some features of psensor can be enabled or disabled before setting up the
> build directories by changing the values of [config.h](src/config.h)
> ### The config is used by all 3 components
