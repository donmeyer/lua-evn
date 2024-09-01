// Lua_EVN.ino

//
// Lua engine for EVN Alpha board.
//

/**
 * EVN Alpha
 */



#include <SPI.h>
#include <SDFS.h>

#include "src/lua_support.h"

#include <EVN.h>
#include "src/lib_evn_board.h"


// 0=SPI0, 1=SPI1
#define SD_CARD_SPI     0


//
// SD Chip-select config.
//
#if SD_CARD_SPI == 0
    // SPI0
    const int SD_TX = 3;
    const int SD_RX = 0;
    const int SD_CS = 1;
    const int SD_SCK = 2;
#else
    // SPI1
    const int SD_TX = 11;
    const int SD_RX = 8;
    const int SD_CS = 9;
    const int SD_SCK = 10;
#endif


// The EVN board object is defined here because defining it as a dynamic object
// via the Lua library doesn't work. (Not sure why)
// The library will use a pointer to this object.
EVNAlpha board( BUTTON_PUSHBUTTON, false );


volatile bool setup0done = false;
volatile bool setup1done = false;



void setup()
{
    LUA_SERIAL.begin( 115200 );

    delay( 1500 );
    LUA_SERIAL.println( "\r\nLua EVN 1.0.3  (EVNAlpha)" );

    board.begin();
    lib_evn_set_board( &board );

   // Ensure the SPI pinout the SD card is connected to is configured properly
#if SD_CARD_SPI == 0
    SPI.setRX( SD_RX );
    SPI.setTX( SD_TX );
    SPI.setSCK( SD_SCK );
    SPI.setCS( SD_CS );
#else
    SPI1.setRX( SD_RX );
    SPI1.setTX( SD_TX );
    SPI1.setSCK( SD_SCK );
    SPI1.setCS( SD_CS );
#endif

    SDFSConfig cfg;
    cfg.setCSPin(SD_CS);
    SDFS.setConfig(cfg);

    if( ! SDFS.begin() )
    {
        LUA_SERIAL.println( "SD card initialization failed!" );
    }

    setupLua();

    setup0done = true;
}


void setup1()
{
    // Wait for setup on core 0 to complete.
    while( setup0done == false )
        ;

    setupLua1();

    setup1done = true;
}


void loop()
{
    // Don't do anything until both core's setup functions have completed.
    if( setup1done )
    {
        runLua();
    }
}
