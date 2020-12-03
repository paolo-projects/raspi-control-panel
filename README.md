# Raspi Control Panel

A control panel written in C++ using the SDL library, designed to run on a 3.5 Raspberry Pi touchscreen.

It aims at simplifying the control of external instrumentation, and collect and optionally display data from it.

The choice of SDL and not other X11 compatible desktop solutions is to avoid the overhead of a desktop and to grant a quick startup at boot.