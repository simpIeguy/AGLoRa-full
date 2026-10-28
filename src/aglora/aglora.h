#ifndef AGLORA_INCLUDED
#define AGLORA_INCLUDED

#include "../settings/settings.h"
#include "../hardware/lora/loraData.h"
#include "../hardware/gps/gps.h"
#include "../utils/memory/sram/sram.h"
#include "../utils/memory/eeprom/eepromaglora.h"

#if defined(ARDUINO_AVR_EBYTE_E32) || defined(ARDUINO_AVR_EBYTE_E220) || defined(ARDUINO_AVR_EBYTE_E22)
  #include "hardware/ble/hm-10.h"
#endif

#if defined(ESP32_C3_EBYTE_E32) || defined(ESP32_C3_EBYTE_E220) || defined(ESP32_EBYTE_E220) || defined(ESP32_EBYTE_E32)
  #include "hardware/ble/esp32-ble.h"
#endif



/*****************************************************
     _       ____   _        ___    ____       _    
    / \     / ___| | |      / _ \  |  _ \     / \   
   / _ \   | |  _  | |     | | | | | |_) |   / _ \  
  / ___ \  | |_| | | |___  | |_| | |  _ <   / ___ \ 
 /_/   \_\  \____| |_____|  \___/  |_| \_\ /_/   \_\
******************************************************/


class AGLORA
{
public:
  AGLORA(IMemory * memory, BLE * ble);
  void hello();
  void checkMemoryToBLE();
  void clearDataPacket(DATA * trackerData);
  void updateSensors(DATA * trackerData);
  void printPackage(LORADATA * loraDataPacket);
  void getRequest(String request);
  void sendPackageToBLE(DATA * trackerData, int index);
  void sendMyPackageToBLE(DATA * trackerData);

private:
  IMemory * _memory;
  BLE * _ble;
  void sendAllPackagesToBLE();
  void sendLastPackagesToBLE();
  void sendPackageToBLEFromStorage(unsigned int index);
  bool isDataMoreRecent(DATA * newData, DATA * oldData);

};


#endif

