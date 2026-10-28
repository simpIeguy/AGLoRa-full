#if defined(ARDUINO_AVR_EBYTE_E32) || defined(ARDUINO_AVR_EBYTE_E220) || defined(ARDUINO_AVR_EBYTE_E22) 


#ifndef BLE_INCLUDED
#define BLE_INCLUDED

#include <Arduino.h>


// ========================== BLE SECTION ==============================
/*
  ____    _       _   _   _____   _____    ___     ___    _____   _   _ 
 | __ )  | |     | | | | | ____| |_   _|  / _ \   / _ \  |_   _| | | | |
 |  _ \  | |     | | | | |  _|     | |   | | | | | | | |   | |   | |_| |
 | |_) | | |___  | |_| | | |___    | |   | |_| | | |_| |   | |   |  _  |
 |____/  |_____|  \___/  |_____|   |_|    \___/   \___/    |_|   |_| |_|
                                                                        
*/


class BLE
{
public:
    BLE();
    void setup();
    String read();
    void send(String * package);

private:
    const byte MTU = 20;
    void sendCommand(const String command);
};

#endif
#endif
