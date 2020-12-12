# Raspi Control Panel

A control panel for the TCD1304 written in C++ using the SDL library, designed to run on a 3.5 Raspberry Pi touchscreen.

It aims at simplifying the control of external instrumentation, and collect and optionally display data from it.

The choice of SDL and not other X11 compatible desktop solutions is to avoid the overhead of a desktop and
to have the interface quickly loaded at boot.

The 3.5" TFT display (a waveshare-like one) grants a discrete amount of pixels to work with (320x480), 
and the touch interaction is quick and responsive.
The animations, however, are sloppy because of the very low refresh rate (around 5 Hz, supply voltage dependent).

This doesn't prevent us from using the display as a control panel to command some external instrument,
and even to display data and charts on it.

The code is not so bad, I created some wrapper classes for graphic objects, touch interaction, scenes.
The scenes are the fundamental part of the interface, and they represent a full screen view.

The scenes are created at the start of the program, and they can be switched at any time by using the SceneManager object
provided in the constructor of each scene.
The touch interaction works by searching for touch enabled objects in the currentScene, and calling a function set in them.
To set the callback function you can call the appropriate method on the object.

The objects can be touch-disabled, and they can propagate the touch event to other
objects placed below them, by setting the properties through the object methods.

The scenes have zero or more objects in them that get drawn by the main loop. The scenes do not draw anything by themselves,
only the children objects get drawn.

The tool has been made to interface with the STM32 firmware made by Esben Rossel 
(available at [https://tcd1304.wordpress.com/](https://tcd1304.wordpress.com/)) through UART communication.
The devices paths are the default ones for the RaspberryPI using the RaspberryPi OS and the 
[3.5 TFT LCD-Show drivers](https://github.com/goodtft/LCD-show) have to be installed. The touch input has to be
at `/dev/input/event0` and the UART serial descriptor at `/dev/serial0`. You may need to change the permissions or run the tool
as root to access the devices. The paths are for now hardcoded but I plan to let them be changed through env vars.

Available graphics objects are, as of now, simple Buttons, ImageButtons, Texts, XYPlots and a couple of cool wave animations.