# Lua EVN Manual

Version 1.1


Copyright (c) 2024, Donald T. Meyer
All rights reserved.


# Overview

Lua EVN makes it possible to write programs for the EVNAlpha board using the Lua language.

Lua is a simple, clean language used often for embedded systems and gaming. It provides memory management
and a rich set of libraries for math, string manipulations, etc.


Lua EVN provides several advantages over programming directly in C/C++.

A) Lua is a higher-level language, and may be more accessible than C/C++.

B) It implements a serial port shell that allows interacting with the running Lua program in multiple ways:

1.  Displaying the value of variables.
2.  Changing the value of variables.
3.  Invoking Lua functions "on the fly".
4.  Modifying and updating the software without the Arduino build/download steps.
5.  Downloading programs is possible "over the air" if a BT similar wireless connection is made to one of the EVNAlpha serial ports.

C) Last but not least, the Lua EVN system allows modifying and updating code *while the program is running*.

This lets you adjust not just various settings and coefficients that may control your robot's behavior, but
to even change program *logic and behavior* without a reboot.


The general structure of a Lua script is similar to the underlying Arduino structure, with a setup() function and then
a loop() function (actually multiple loops).

The `Lua Examples` folder contains several useful example scripts.


-------------------------------------------------------------
<br>

# State of the Project <br>(2-Sep-2024)

Currently, only some of the EVN Alpha hardware objects are supported:
* Motor
* Servo
* ContinuousServo
* Drivebase

* DistanceSensor
* CompassSensor
* IMUSensor  (UNTESTED)
* ColourSensor

* Display
* Matrix LED
* RGB LED
* 7-Segment Display (UNTESTED)

The rest are in work.


The `arduino` library is also only partially complete, with support for:
* Digital I/O
* Analog I/O
* Time

The Advanced I/O and Communication functions are also in-work.
(Note that the other standard Arduino functions are, for the most part, provided by the
Lua platform itself, and will not be part of the Lua Arduino library.)


-------------------------------------------------------------
<br>

# Quick Start

To get the minimal example of the Lua EVN system running, these are the steps that are required:

1.  Place the Lua_EVN folder into the Arduino source folder.
2.  Download, configure, and install the Lua source code into the Arduino `libraries` folder. (See: Lua Library for Arduino)
3.  Build and install the Lua EVN application onto the EVNAlpha board using the Arduino IDE or Arduino-CLI.
4.  Connect a serial terminal to the EVNAlpha board.
5.  Use the `*<return>` command in the Lua EVN shell to perform an anonymous download and run the example script `Lua Examples/main_simple.lua`.


-------------------------------------------------------------
<br>


# Installation and Setup

Note that the Arduino system and EVN Alpha libraries are installed and working.


## Filesystem

Lua EVN expects to store Lua script files on a file system.

This can either be an external SD card (recommended), or by setting up a virtual filesystem in the EVNAlpha onboard Flash.

Note that the Lua EVN system can be used *without* an SD card, although with limitations.
See the section "No SD Card"


### SD Card Hardware

As written, the Lua EVN system expects an SD card connected via SPI1.
*(See the EVNAlpha docs for pin connection details.)*

This will use the pins for Servo1, Servo2, and Serial 1.

The SD card should be formatted with a FAT filesystem.

The hardware used in development was an Adafruit breakout board: [Adafruit MicroSD Breakout](https://www.adafruit.com/product/254).
*(I would expect any hardware that allows connecting an SD card to the EVNAlpha would be fine.)*


### Virtual Filesystem

TBD


## Lua Library for Arduio

This application has been developed and tested using **Lua 5.4.7**.

The source code for Lua is not part of this Lua EVN distribution, and must be obtained separately. Follow these steps to obtain and configure the Lua source:

**Obtain the source code**
Download the Lua source code from <https://www.lua.org/download.html>

**Add the Lua code as an Arduino library**
Extract the source code from the tar/zip file, and place the contents of the zip file's `src` folder into a library folder in your Arduino source directory.
(For example, `/Arduino/libraries/Lua`, which should directly contain the Lua source files -- they should not be in a `src` sub-folder, so the directory tree should, for example, look like `/Arduino/libraries/Lua/<.c and .h files>`.)

**Modify the Lua source code**
Delete these files from the Lua source distribution: `lua.c` and `luac.c`.
*(They aren't needed in the application, and will cause build errors if present.)*

In the `luaconf.h` file, uncomment the following define (at or near line 44):
`#define LUA_USE_C89`

Make the following edits to the `luaconf.h` file:
Near the beginning of the file, at around line 31, add the following block of code:

    // Allow the application to route output text as desired.
    void dtm_writestring( const char *p, int len );
    #define lua_writestring(s,l)   dtm_writestring(s,l)

    void dtm_writestringerror( const char *s, const char *p );
    #define lua_writestringerror(s,p)    dtm_writestringerror(s,p)


## Lua EVN Application

Add the `Lua_EVN` folder into your Arduino source folder.
(For example, `/Arduino/Lua_EVN`.)

Build using the Arduino IDE, or using Arduino CLI.


### Debug Output
By default, some debug output is sent to the Lua shell terminal. This can be disabled by changing `#define DEBUG_ENABLED` to 0 in the file `lua_support.cpp`.


-------------------------------------------------------------
<br>

# Shell Configuration and Connection

By default, the Lua EVN shell is connected to `Serial`.
This can be changed to a different serial port by editing the define `#define LUA_SERIAL    Serial` in the header file `lua_support.h`.

The baud rate is 115,200. This can be changed in the `Lua_EVN.ino` file.

You may need to set your terminal program to consider a standalone LF character to be a CRLF.

Robots often move, so a wireless connection is really useful for Lua EVN, since you can check variables,
change them and even reload code remotely if you have a wireless connection.

My setup is using two SparkFun [BlueSMiRF v2](https://www.sparkfun.com/products/23287) boards, but other BT serial boards, XBee, etc. should also work.


-------------------------------------------------------------
<br>

# Downloads


Lua script files may be placed onto the SD card two ways. The SD card can be connected to a computer and the files copied onto it in the normal fashion.
The second method is by sending them over the serial port used for the Lua EVN shell.

The download command is entered at the shell prompt (e.g. `*main`). The shell begins download mode, and expects the new contents of the Lua file to be sent as text.

There is no error checking or correction, so the serial connection must be reliable!

There is no timeout on when the download needs to commence.
The download is consider complete when, after the first character is received, one second passes with no additional characters being sent from the terminal.

See the "Shell Commands" section below for details on the available download commands.

Most terminal programs should have the ability to "send" a plan text file.

Note that if the serial connection is extremely fast, some amount of line or character pacing delay may be required to prevent the file contents from being corrupted.
I've had good luck with a 0.005 second character delay.

-------------------------------------------------------------
<br>


# Script Structure

### Main Script
The Lua EVN system expects that a script file named `main.lua` is present on the filesystem,

At bootup, this file is loaded and executed. It may, in turn, include other script files via `require`.

### Setup Functions
Once, after the file is initially loaded, if functions named `setup()` and `setup1()` are present, they will be run.`
These will NOT be called again should the `main.lua` script be re-downloaded.

### Loop Functions
The expectation is that there will usually be a Lua function named `exec_loop()`, and possibly one named `housekeeping_loop()`.
These will be called repeatedly by the Lua EVN system.

These functions should, as much as possible, not block to allow the Lua EVN shell to be responsive.

The only difference between how these loop functions are called and used is their names. Loop execution can be disabled if desired via shell commands (e.g. `-e`),
and the two loops exist to allow stopping one loop while the other continues to run.

For example, you may want sensors to keep being read and logged, or some robot safety behaviors to continue in the Housekeeping loop, but
temporarily suspend some behavioral code running in the Exec loop while changes or made, variables are examined and adjusted, etc.

Note that if there is an error when executing a loop, that loop will be suspended to avoid error messages from "spamming" the shell.
If a corrected file or function is downloaded, the loop must be manually re-enabled. (e.g. `+e`)


### No SD Card

It is possible to use the Lua EVN system without any type of filesystem.

While this is not the primary design intent, it can serve well for evaluation, and early development.

Without a filesystem, some important capabilities are lost, such as retaining the Lua script across reboots,
being able to break the program up into multiple Lua script files, and being able to place the motor
and servo `begin()` calls onto the second processor core.

The system will boot just fine without an SD card. Then, using the "anonymous chunk" download command `*`, a Lua script can be downloaded.

There is one accommodation made for this scheme, and that is to have the `setup()` function called for this anonymous script, even
though it was not present at bootup (since there was no script at all at bootup!)

If the initial bootup find no `setup()` function, the first time one is detected after an anonymous script is downloaded,
or even a new `main.lua` is downloaded to an SD card, it will be automatically called.


-------------------------------------------------------------
<br>


# Shell Usage

The shell can be used to do two things:
1. Control the Lua EVN system
2. Execute Lua code


The shell commands below are processed by the platform, and allow doing things like
download new files, or enable/disable program loop execution.

For example:

    > *foo          <-- Downloads a file to the module foo.lua


All other text entered will be sent to the Lua interpreter. This allows running snippets of
Lua code, or entering entire functions.

If a Lua statement is executed and produces return values, these will be printed out by the Lua EVN platform.
This makes it convenient to display the value of a given variable, or the return from a
function.

For example:

    > x = 77
    > print(x)      <-- prints "77"
    > x             <-- also prints "77"

### Multiline Statements

A Lua chunk that spans multiple lines can be entered like this:

    > function twice(a)
    >> return a * 2
    >> end
    >

### File Download Convenience Feature

Files (Lua modules) can have their download triggered by some of the commands documented below.
There is also a convenience feature that keys off of the first line in a Lua file.

If the first line begins with:
    -- <name>.lua

Then a download to the file of the given name will begin when that file is sent down to the
EVN board, with no download command needing to be manually typed.

Example file:

    -- test.lua
    --
    ... more lua code...

When sent to the EVN Lua system from connected terminal software, this will automatically trigger a
download of the file `test.lua`.


## Commands

#### `*`<br>Download an anonymous Lua chunk.
This chunk of Lua code (which can be an entire script file) is loaded and executed.
The contents are not written to a file, and will be lost when the system reboots.


#### `*<module>` <br> Download a Lua module to a file.
If the file exists, it will be updated. If not, it will be created.
The extension ".lua" will be added.

Not that the name is the name of the Lua module that will be created on the EVN Alpha board's filesystem, it
is not the name of the file on the connected computer, and the name does NOT need to match.

For example, file `Foo1.lua` on the computer can be downloaded as `Bar.lua` on the EVN board.


#### `-- *<module>.lua` <br> Download a Lua module to a file.
If the file exists, it will be updated. If not, it will be created.

This isn't really a command, it is more of a convenient hack to let the first line of a file being
sent to the EVN board trigger the download process and tell the Lua EVN system what the
name of the file being downloaded should be.


#### `@<module>` <br> Reload the given module.
The extension ".lua" will be added.

(For example, after replacing the SD card or SD card file via outside methods.)


#### :+e <br> Enable the Executive loop
exec_loop()

#### :-e <br> Disable the Executive loop
exec_loop()

#### :+e <br> Enable the Housekeeping loop
housekeeping_loop()

#### :-e <br> Disable the Housekeeping loop
housekeeping_loop()


-------------------------------------------------------------
-------------------------------------------------------------


# Libraries

<br>
## Lua Platform

Library name: "luaplatform"

This library gives access to features of the Lua system platform itself.

### setExecEnabled
Enable and disable the Executive loop

`setExecEnabled( bool )`

### setHousekeepingEnabled
Enable and disable the Housekeeping loop

`setHousekeepingEnabled( bool )`

### execEnabled
Read the state of the Executive loop

`bool execEnabled()`

### housekeepingEnabled
Read the state of the Housekeeping loop

`bool housekeepingEnabled()`

-------------------------------------------------------------

## EVN

Library name: "evn"

This library contains the EVN Board functions, as well as the means to dynamically create EVN hardware objects
such as motors and sensors.

The board functions are not methods on a board object, unlike the hardware objects which are instances of their specific classes.

They are named the same as in the EVN C++ library, for example:

    evn = require "evn"
    v = evn.getBatteryVoltage()
    print(v)


Object creation is done by using the hardware device name minus the EVN prefix.

    evn = require "evn"
    m1 = evn.Motor.new( 1, Motor.EV3_MED )


The methods of each hardware object also match the names and functionality of the C++ library versions.

    m1:runTime( 10, 2000, Motor.STOP_COAST, false )



-------------------------------------------------------------

## Arduino

Library name: "arduino"

This library contains the standard Arduino functions that relate to hardware.
Math, trig, character functions etc. are not included, as Lua has its own versions
of most if not all of those functions.

    ard = require "arduino"
    ard.pinMode( 10, ard.OUTPUT )
    ard.digitalWrite( 10, ard.HIGH )

