-- main_simple.lua
--
-- The script named "main.lua" is always loaded by the Lua system at bootup.
-- This should be renamed as "main.lua" when copied to the SD card, or downloaded via the
-- Lua shell aas "main".
--
--
-- Simple Lua EVN script example
--
-- Can be run without any additional hardware attached to the EVNAlpha board.
-- This will blink the onboard LED.
--


print( "Loading the main script" )


-- Arduino and EVN libraries (built in 'C' libraries)
ard = require "arduino"
evn = require "evn"

print( "Simple Main script loaded, Revision 1.0" )


-- Perform the initial program setup.
--
-- The setup() function runs only the first time the file is loaded.
-- Subsequent updates while the system is running won't execute this again.
function setup()
    print( "Running the script setup() function in Lua!" );

    -- Variables defined here will survive a script reload, since this function will not be re-executed.
end


-- The exec_loop() function is called repeatedly (the equivalent of the Arduino loop() function).
-- It should return as quickly as possible to keep the interactive shell responsive.
-- Note that both the exec and housekeeping loops run on core0, and if one loop blocks, the other won't run
-- until the blocking loop returns.
function exec_loop()
end


tick = 0
ledstate = false

-- The housekeeping_loop() function is called repeatedly (the equivalent of the Arduino loop() function).
-- It should return as quickly as possible to keep the interactive shell responsive.
-- Note that both the exec and housekeeping loops run on core0, and if one loop blocks, the other won't run
-- until the blocking loop returns.
function housekeeping_loop()
    if ard.millis() > (tick + 1000) then
        ledstate = not ledstate

        evn.ledWrite( ledstate )

        tick = ard.millis()
    end
end
