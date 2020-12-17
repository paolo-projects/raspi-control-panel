# Raspi Control Panel

A control panel for the TCD1304 written in C++ using the SDL library, designed to run on a 3.5 Raspberry Pi touchscreen.

The tool has been made to interface with an STM32 board running the firmware made by Esben Rossel (available at [https://tcd1304.wordpress.com/](https://tcd1304.wordpress.com/)) through UART communication.

The firmware expects a 12 bytes data packet with the parameters needed to control the CCD, then it will send the ADC readings serialized in an array of 16 bit values. Refer to [Esben Rossel's blog](https://tcd1304.wordpress.com/) for a thorough explanation of the whole process.

The tool will just plot the XY values received by the board, as the data processing code is still undone.

The devices paths are the default ones for the RaspberryPI using the RaspberryPi OS, and the [3.5 TFT LCD-Show drivers](https://github.com/goodtft/LCD-show) has to be installed.
The touch input device is hardcoded to `/dev/input/event0` and the UART serial descriptor to `/dev/serial0`.
You may need to change the permissions or run the tool as root to read/write to the devices.

This tool aims at simplifying the control of the CCD, collect and display data from it.
The choice of SDL and not other X11 compatible desktop solutions is to avoid the overhead of a desktop and to have the interface quickly loaded at boot.

The 3.5" TFT display (a waveshare-like one) grants a discrete amount of pixels to work with (320x480), and the touch interaction is quick and responsive.
The animations, however, are sloppy because of the very low refresh rate (around 5 Hz, supply voltage dependent).

<img src="https://github.com/paolo-projects/raspi-control-panel/raw/master/pictures/setup.jpg" width="520" height="300" />

The project has been refactored and the GUI code now exists as a library dependency: [https://github.com/paolo-projects/TouchCPLib](https://github.com/paolo-projects/TouchCPLib).

# Compiling

There's a VisualStudio project included for remote compilation on a Raspberry, and there's a Makefile in the `StlTest` folder

To compile the application you need to compile and install the TouchCP library available [here](https://github.com/paolo-projects/TouchCPLib).

The executable expects to find some stuff in the current working directory:

- a font named `font.ttf`
- a bunch of png images to use as icons: `laser.png`, `logout.png`, `left-arrow.png`, `scale.png`, `settings.png`, `test-tube.png`

Either provide your own or change the source code to your needs.