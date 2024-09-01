-- main_anon.lua
--
--
-- Example code that creates motors, displays, and sensors.
--
-- Can be loaded as an anonymous Lua chunk from the Lua EVN shell, and does not require an SD card
--
-- Hardware needed:
--  Medium EV3 motor on port 1
--  Servo on port 4
--  EVN Distance sensor on port 1
--  EVN Display on port 2
--
-- Note that motors and servos are being initilaized on core0 in setup(), rather
-- than how they would normally be on core1 in the setup1() function.
-- (which does not run when a chunk is loaded anonymously due to no SD card being present.)

print( "Loading the main script" )


-- Arduino and EVN libraries (built in 'C' libraries)
ard = require "arduino"
evn = require "evn"


-- Convenience aliases for EVN classes
Motor = evn.Motor
Servo = evn.Servo


print( "Main anon script loaded, Revision 1.0" )


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

    -- Create a servo object
    s4 = Servo.new( 4 )

    -- The begin() method for motors and servos is called here since we are being loaded without a SD card.
    m1:begin()
    m2:begin()
    s4:begin()

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
