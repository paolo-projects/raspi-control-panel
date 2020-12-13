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

<img src="https://github.com/paolo-projects/raspi-control-panel/raw/master/examples/setup.jpg" width="520" height="300" />

The code is not so bad, I tried to document it extensively. I created some wrapper classes for graphic objects, touch interaction, scenes.
The scenes are the fundamental part of the interface. They represent a full screen view, and you are supposed to subclass the GraphicsScene class to create your own.

The scenes can be switched at any time by using the SceneManager object accessible through the global Application object.

```c++
SceneManager* sM = Application::getCurrentSceneManager(); // or Application::getCurrent()->getSceneManager()
```

The touch interaction works by searching for touch enabled objects in the currentScene, and calling a function set in them.
To set the callback function you can call the appropriate method on the object

```c++
button->setTouchCallback([](){ ... });
```

The objects can be touch-disabled, and they can propagate the touch event to other objects placed below them, by setting the properties through the object methods.

The scenes have zero or more objects in them that get drawn by the main loop.
The scenes do not draw anything by themselves, only the children objects get drawn.

It also features a Worker thread so that you can run code independently from the main UI thread. You do it by wrapping your function in a Task and calling the `runOnWorkerThread` method on the application object

```c++
Application::runOnCurrentWorkerThread(new Task(...)) // it's a shortcut for Application::getCurrent()->runOnWorkerThread(...)
```

To run code on the main thread (UI thread), so as to update UI elements, you run a task using the `runOnMainThread` method.

Available graphics objects are, as of now, simple Buttons, ImageButtons, Texts, XYPlots and a couple of cool wave animations.

# Compiling

There's a VisualStudio project included for remote compilation on a Raspberry, and there's a Makefile in the `StlTest` folder

To compile the application you need the following libraries:

- libsdl2
- libsdl2-ttf
- libsdl2-image
- libts

Install them and then run `make`

The executable expects to find some stuff in the current working directory:

- a font named `font.ttf`
- a bunch of png images to use as icons: `laser.png`, `logout.png`, `left-arrow.png`, `scale.png`, `settings.png`, `test-tube.png`

Either provide your own or change the source code to your needs.