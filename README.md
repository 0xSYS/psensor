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
