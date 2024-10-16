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


## Installation requirements:

### Meson build system
`sudo dnf install meson ninja-build`

### Dependencies

| Linux Distro    	| Development Packages        	| Default Packages     	| Used for                                                             	|
|-----------------	|-----------------------------	|----------------------	|----------------------------------------------------------------------	|
| Fedora 39 / 40  	| gtk3-devel                  	| gtk3                 	| Graphical User Interface                                             	|
|                 	| libnotify-devel             	| libnotify            	| Desktop notification (As warnings)                                   	|
|                 	| libcurl-devel               	| libcurl              	| Getting files from web servers                                       	|
|                 	| libunity-devel              	| libunity             	| Integration with unity and plasma                                    	|
|                 	| libappindicator-devel       	| libappindicator      	|                                                                      	|
|                 	| pango-devel                 	| pango                	| Layout system and rendering internationalized text                   	|
|                 	| cairo-devel                 	| cairo                	| 2D graphics library                                                  	|
|                 	| gdk-pixbuf2-devel           	| gdk-pixbuf2          	| Image parsing library                                                	|
|                 	| atk-devel                   	| atk                  	| Interfaces for accessibility support                                 	|
|                 	| ghc-gi-freetype2-devel      	| ghc-gi-freetype2     	|                                                                      	|
|                 	| harfbuzz-devel              	| harfbuzz             	| Text shaping library                                                 	|
|                 	| glib-devel                  	| glib                 	|                                                                      	|
|                 	| dbus-devel                  	| dbus                 	|                                                                      	|
|                 	| fribidi-devel               	| fribidi              	|                                                                      	|
|                 	| libxml2-devel               	| libxml2              	|                                                                      	|
|                 	| pixman-devel                	| pixman               	|                                                                      	|
|                 	| libpng-devel                	| libpng               	|                                                                      	|
|                 	| libblkid-devel              	| libblkid             	|                                                                      	|
|                 	| sysprof-capture-devel       	| * Development only * 	|                                                                      	|
|                 	| p11-kit-devel               	| p11-kit              	|                                                                      	|
|                 	| lm_sensors-devel            	| lm_sensors           	| Retirive general sensor information                                  	|
|                 	| json-c-devel                	| Json-c               	| Used to create a json serialised data returned from the core library 	|
|                 	| glib2-devel                 	| glib2                	|                                                                      	|
|                 	| libgtop2-devel              	| libgtop2             	| Retriving CPU usage and available ram                                	|
|                 	| libudisks2-devel            	| libudisks2           	| Disk information provider                                            	|
|                 	| libatasmart-devel           	| libatasmart          	| Disk information provider                                            	|
|                 	| libXNVCtrl-devel            	| libXNVCtrl           	| Providing NVidia GPU information                                     	|
| Debian / Ubuntu 	| libglibgtk-3-dev                	| libgtk-3-0           	|                                                                      	|
|                 	| libnotify-dev               	| libnotify4           	|                                                                      	|
|                 	| libcurl4-openssl-dev        	| libcurl4             	|                                                                      	|
|                 	| libunity-dev                	| libunity9            	|                                                                      	|
|                 	| libappindicator3-dev        	| libappindicator3-1   	|                                                                      	|
|                 	| libpango1.0-dev             	| libpango-1.0-0       	|                                                                      	|
|                 	| libcairo2-dev               	| libcairo2            	|                                                                      	|
|                 	| libgdk-pixbuf-2.0-dev       	| libgdk-pixbuf-2.0-0  	|                                                                      	|
|                 	| libatk1.0-dev               	| libatk1.0-0          	|                                                                      	|
|                 	| libfreetype-dev             	| libfreetype6         	|                                                                      	|
|                 	| libharfbuzz-dev             	| libharfbuzz0b        	|                                                                      	|
|                 	| libglib2.0-dev              	| libglib2.0-0         	|                                                                      	|
|                 	| libdbus-1-dev               	| libdbus-1-3          	|                                                                      	|
|                 	| libfribidi-dev              	| libfribidi0          	|                                                                      	|
|                 	| libxml2-dev                 	| libxml2              	|                                                                      	|
|                 	| libpixman-1-dev             	| libpixman-1-0        	|                                                                      	|
|                 	| libpng-dev                  	| libpng16-16          	|                                                                      	|
|                 	| libblkid-devel              	| libblkid1            	|                                                                      	|
|                 	| libsysprof-4-dev            	| libsysprof-4         	|                                                                      	|
|                 	| libp11-kit-dev              	| libp11-kit0          	|                                                                      	|
|                 	| libsensors-dev              	| libsensors5          	|                                                                      	|
|                 	| libjson-c-dev               	| libjson-c5           	|                                                                      	|
|                 	| libglib2.0-dev              	| libglib2.0-0         	|                                                                      	|
|                 	| libgtop2-dev                	| libgtop2-common      	|                                                                      	|
|                 	| libudisks2-dev              	| libudisks2-0         	|                                                                      	|
|                 	| libatasmart-dev             	| libatasmart4         	|                                                                      	|
|                 	| libxnvctrl-dev              	| libxnvctrl0          	|                                                                      	|
| Arch linux      	| gtk3                        	| -                    	|                                                                      	|
|                 	| libnotify                   	| -                    	|                                                                      	|
|                 	| libcurl (Failed to install) 	| -                    	|                                                                      	|
|                 	| libunity                    	| -                    	|                                                                      	|
|                 	| libappindicator-gtk3        	| -                    	|                                                                      	|
|                 	| pango                       	| -                    	|                                                                      	|
|                 	| cairo                       	| -                    	|                                                                      	|
|                 	| gdk-pixbuf2                 	| -                    	|                                                                      	|
|                 	| at-spi2-core                	| -                    	|                                                                      	|
|                 	| freetype2                   	| -                    	|                                                                      	|
|                 	| harfbuzz                    	| -                    	|                                                                      	|
|                 	| glib2                       	| -                    	|                                                                      	|
|                 	| dbus                        	| -                    	|                                                                      	|
|                 	| fribidi                     	| -                    	|                                                                      	|
|                 	| libxml2                     	| -                    	|                                                                      	|
|                 	| pixman                      	| -                    	|                                                                      	|
|                 	| libpng                      	| -                    	|                                                                      	|
|                 	| libblkid (Unknown)          	| -                    	|                                                                      	|
|                 	| libsysprof-capture          	| -                    	|                                                                      	|
|                 	| libp11-kit                  	| -                    	|                                                                      	|
|                 	| lm_sensors                  	| -                    	|                                                                      	|
|                 	| json-c                      	| -                    	|                                                                      	|
|                 	| glib2                       	| -                    	|                                                                      	|
|                 	| libgtop                     	| -                    	|                                                                      	|
|                 	| udisks2                     	| -                    	|                                                                      	|
|                 	| libatasmart                 	| -                    	|                                                                      	|
|                 	|                             	|                      	|                                                                      	|

Fedora:
```
sudo dnf update
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
libatasmart-devel
```

Debian / Ubuntu:
```
sudo apt update
sudo apt upgrade
sudo apt install libgtk-3-dev \
libnotify-dev \
libcurl4-openssl-dev \
libunity-dev \
libappindicator3-dev \
libpango1.0-dev \
libcairo2-dev \
libgdk-pixbuf-2.0-dev \
libatk1.0-dev \
libfreetype-dev \
libharfbuzz-dev \
libglib2.0-dev \
libdbus-1-dev \
libfribidi-dev \
libxml2-dev \
libpixman-1-dev \
libpng-dev \
libblkid-dev \
libsysprof-4-dev \
libp11-kit-dev \
libsensors-dev \
libjson-c-dev \
libglib2.0-dev \
libgtop2-dev \
libudisks2-dev \
libatasmart-dev \
libxnvctrl-dev
```

Arch Linux:
```
sudo pacman -Syu
sudo pacman -Sy gtk3 \
libnotify \
curl \
libunity \
libappindicator-gtk3 \
pango \
cairo \
gdk-pixbuf2 \
at-spi2-core \
freetype2 \
harfbuzz \
glib2 \
dbus \
fribidi \
libxml2 \
pixman \
libpng \
util-linux \
libsysprof-capture \
libp11-kit \
lm_sensors \
json-c \
glib2 \
libgtop \
udisks2 \
libatasmart
```

## Step 2

```
git clone -b meson-build-syst https://github.com/0xSYS/psensor
```

Buldding the core library first is an optional step as I added the even the sources to the library.
So the library and the utilities (GUI and web server) will build at the same time

## Building GUI utility
```
cd src/GUI
meson setup builddir
meson compile -C build dir
```

> [!IMPORTANT]
> Before executing the binary you need to run `sudo bash scripts/install-resources`. \
> - Copies all glade files needed by psensor in `/usr/local/share/psensor`
> - Copies the psensor.gschema.xml file to `/usr/share/glib-2.0/schemas`
> - Updates gschemas.compiled file

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
