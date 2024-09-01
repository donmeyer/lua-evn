# lua-evn

A Lua system for the [EVN Alpha](https://coresg.tech/evn/) platform.


Lua EVN makes it possible to write programs for the EVNAlpha board using the Lua language.

Lua is a simple, clean language used often for embedded systems and gaming. It provides memory management
and a rich set of libraries for math, string manipulations, etc.


Lua EVN provides several advantages over programming directly in C/C++.

A) Lua is a higher-level language, and may be more accessible than C/C++.

B) It implements a serial port shell that allows interacting with the running Lua program in multiple ways:

2.  Displaying the value of variables.
3.    Changing the value of variables.
4.    Invoking Lua functions "on the fly".
5.    Modifying and updating the software without the Arduino build/download steps.
6.    Downloading programs is possible "over the air" if a BT similar wireless connection is made to one of the EVNAlpha serial ports.

C) Last but not least, the Lua EVN system allows modifying and updating code *while the program is running*.

This lets you adjust not just various settings and coefficients that may control your robot's behavior, but
to even change program *logic and behavior* without a reboot.


The general structure of a Lua script is similar to the underlying Arduino structure, with a setup() function and then
a loop() function (actually multiple loops).

The `Lua Examples` folder contains several useful example scripts.


**This is a work in progress.**

Currently, only some of the EVN Alpha hardware objects are supported, and the `arduino` library is also only partially complete.
See the `Manual.md` document for detailed status information.
