-- main_sd.lua
--
-- The script named "main.lua" is always loaded by the Lua system at bootup.
-- This should be renamed as "main.lua" when copied to the SD card, or downloaded via the
-- Lua shell aas "main".
--
--
-- Example code that creates motors, displays, and sensors.
--
-- Expects to run from the SD card filesystem for two reasons:
--    1) Uses `require` to load the "shell.lua" script from the SD card.
--    2) Initializes the motors and servo in the `setup1()` function, which
--       is only called when `main.lua` is run from the SD card at bootup.
--
-- Hardware needed:
--  Medium EV3 motor on port 1
--  Servo on port 4
--  EVN Distance sensor on port 1
--  EVN Display on port 2
--
-- Also required, the "shell.lua" script being present on the SD card.


print( "Loading the main script" )


-- Global functions handy when using an interactive shell
-- These go into the global namespace.
require "shell"

-- Arduino and EVN libraries (built in 'C' libraries)
ard = require "arduino"
evn = require "evn"


-- Convenience aliases for EVN classes
Motor = evn.Motor
Servo = evn.Servo


print( "Main SD script loaded, Revision 1.0" )


-- Global variables created in the setup functions:
-- m1
-- m2
-- s3
-- d1
-- dis

-- Perform the initial program setup.
--
-- The setup() function runs only the first time the file is loaded.
-- Subsequent updates while the system is running won't execute this again.
function setup()
    print( "Running the script setup() function in Lua!" );

    -- Variables defined here will survive a script reload, since this function will not be re-executed.

    -- Create a couple of motor objects
    m1 = Motor.new( 1, Motor.EV3_MED )

    m2 = Motor.new( 2, Motor.EV3_MED )
    -- The begin() method for motors is called in setup1() to run on the second core.

    -- Create a servo object
    s4 = Servo.new( 4 )
    -- The begin() method for servos is called in setup1() to run on the second core.


    -- Create a distance sensor object
    -- Didn't create a convenience alias for this class, but could have.
    d1 = evn.DistanceSensor.new( 1 )
    d1:begin()


    -- Create a display object
    --
    -- Note that new() is called using the "." operator, since it is
    -- a library function.
    dis = evn.Display.new( 2 )
    -- The begin() method is called using the ":" operator, as it is a
    -- a class method.
    dis:begin()

    dis:splashEVN()
    dis:clear()
    dis:writeLabel( 0, "Looptime: " )
end


-- The setup1() function runs only the first time the file is loaded.
-- Subsequent updates while the system is running won't execute this again.
function setup1()
    print( "Running the script setup1() function in Lua!" );

    m1:begin()
    m2:begin()

    s4:begin()
end


-- Track the distance so we can update only when a change is detected that is above the noise.
distance = -1

servopos = 180

-- The exec_loop() function is called repeatedly (the equivalent of the Arduino loop() function).
-- It should return as quickly as possible to keep the interactive shell responsive.
-- Note that both the exec and housekeeping loops run on core0, and if one loop blocks, the other won't run
-- until the blocking loop returns.
function exec_loop()
    -- Only if the motor is not currently running
    if m1:completed() then
        -- If the user button is pressed
        if evn.buttonRead() then
            print("run motor")
            m1:runTime( 10, 2000, Motor.STOP_COAST, false )

            s4:write( servopos, 0, 90 )
            if( servopos == 0 ) then
                servopos = 180
            else
                servopos = 0
            end
        end
    end

    local d = d1:read( false )
    -- Use some hysteresis to avoid printing the distance due to noise in the readings
    local delta = math.abs( d - distance )
    if delta >= 7 then
        print( string.format( "Distance: %4d", d ) )
        distance = d
    end
end


tick = 0
ledstate = false

lastloop = 0

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

    local ct = ard.millis()
    local t = ct - lastloop
    dis:writeData( 0, t )
    lastloop = ct
end

-- ==================================================================
