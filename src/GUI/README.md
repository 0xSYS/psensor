# PSensor GUI


It displays a curve for each sensor, and uses Desktop Notification to
raise an alarm when a temperature is too high. On Ubuntu an
Application Indicator is also available, its icon changes when a
temperature alert is raised.


### It can monitor:
* the temperature of the motherboard and CPU sensors (using lm\-sensors).
* the temperature of the NVidia GPUs (using XNVCtrl).
* the temperature and fan rotation speed of the ATI GPUs.
* the temperature of the Hard Disk Drives (using hddtemp or atasmart library).
* the rotation speed of the fans (using lm\-sensors).
* the sensors of a remote computer (using psensor\-server).


> [!IMPORTANT]
> Psensor requires lm\-sensors to be correctly installed and configured,
> it can be checked by running the command 'sensors'. If it has never be
> done, you may need to run the command 'sensors\-detect' and follow the
> instruction. See the manpages of sensors(1) and sensors\-detect(8) for
> more information.

> [!NOTE]
> To retrieve the temperature of the Hard Disk Drives, the hddtemp
> daemon must be running or atasmart used ('\-\-use\-libatasmart').
>
> For remote monitoring:
> * start psensor\-server(1) on the remote computer
> * run psensor with '\-\-url' option: 'psensor \-\-url=http://localhost:3131'
>
> ATI/AMD GPUs monitoring is available if the library libatiadlxx is
> present in the directory /usr/lib and psensor has been compiled with
> the ATI ADL SDK.
> Log messages are written to '$HOME/.psensor/log'.
>
> By default, if a Psensor process is already running, the command
> psensor will show the graph window then exit.
> It is possible to run multiple instances of Psensor by using the
> option '\-\-new\-instance'.


<a id="How To build"></a>
# How To build

## installation requirements

* meson build
Fedora:
`sudo dnf install meson ninja-build`


## Dependencies
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
lm_sensors-devel
```

## Configuring & building:
```
cd GUI
meson setup builddir
meson compile -C builddir
```
