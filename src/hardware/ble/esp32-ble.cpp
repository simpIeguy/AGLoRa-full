/*
Project AGLoRa (abbreviation of Arduino + GPS + LoRa)
Tiny and chip LoRa GPS tracker

https://github.com/Udj13/AGLoRa/

Copyright © 2021-2025 Eugeny Shlyagin. Contacts: <shlyagin@gmail.com>
License: http://opensource.org/licenses/MIT

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty 

*/

#include "../../settings/settings.h"

#if defined(ESP32_C3_EBYTE_E32) || defined(ESP32_C3_EBYTE_E220) || defined(ESP32_EBYTE_E32) || defined(ESP32_EBYTE_E220)

#include <Arduino.h>
#include "esp32-ble.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>



BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
uint8_t value = 0;

String package = "";

// onWrite() runs in the BLE task, read() runs in loop(): share a fixed buffer under a lock
#define RECEIVED_PACKAGE_SIZE 64
char receivedPackage[RECEIVED_PACKAGE_SIZE] = "";
portMUX_TYPE receivedPackageMux = portMUX_INITIALIZER_UNLOCKED;

// #define SERVICE_UUID "0000ffe0-1cda-43b5-984e-61077683f0ab"
// #define CHARACTERISTIC_UUID "0000ffe1-48ea-47d8-af50-100b9134387d"

#define SERVICE_UUID "FFE0"
#define CHARACTERISTIC_UUID "FFE1"


// Manufacturer data (optional)
#define MANUFACTURER_ID 0xFFFF  // Fake ID for example
uint8_t manufacturerData[5] = {0x01, 0x02, 0x03, 0x04, 0x05};  // Custom payload



class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
    Serial.println("Device connected");
    pServer->getAdvertising()->stop();
  };

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
    Serial.println("Device disconnected");
    Serial.println("Restart advertising");
    pServer->getAdvertising()->start();
  }
};

class MyBLECallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string rxValue = pCharacteristic->getValue();

    portENTER_CRITICAL(&receivedPackageMux);
    strncpy(receivedPackage, rxValue.c_str(), RECEIVED_PACKAGE_SIZE - 1);
    receivedPackage[RECEIVED_PACKAGE_SIZE - 1] = '\0';
    portEXIT_CRITICAL(&receivedPackageMux);

    #if DEBUG_MODE && DEBUG_BLE
    Serial.print(F("📲 BLE: 📫 Received: "));
    Serial.println(rxValue.c_str());
    #endif

  }
};

void bleInit();

BLE::BLE()
{
}

void BLE::setup()
{
    bleInit();

#if DEBUG_MODE && DEBUG_BLE
    Serial.print(F("📲[BLE: ready for work ✅. Maximum Transmission Unit (MTU) = "));
    Serial.print(MTU);
    Serial.println(F("]"));
#endif
}

String BLE::read()
{
    char buffer[RECEIVED_PACKAGE_SIZE];

    portENTER_CRITICAL(&receivedPackageMux);
    strcpy(buffer, receivedPackage);
    receivedPackage[0] = '\0';
    portEXIT_CRITICAL(&receivedPackageMux);

    String result = buffer;
    result.trim();

    return result;
}


void BLE::send(String *package)
{
#ifdef USE_BLE_MTU
  sendWithMTU(package);
#else
  sendWithoutMTU(package);
#endif

}

void BLE::sendWithoutMTU(String *package)
{
#if DEBUG_MODE && DEBUG_BLE
    Serial.print(F("📲BLE 📫 Sending: "));
    Serial.print(*package);
#endif

    if (deviceConnected) {
    #if DEBUG_MODE && DEBUG_BLE
        Serial.print(F("   BLE device connected. Package: "));
        Serial.println(package->c_str());
    #endif
        pCharacteristic->setValue(package->c_str());
        pCharacteristic->notify();
        delay(30);                
    }

}

void BLE::sendWithMTU(String *package)
{
#if DEBUG_MODE && DEBUG_BLE
    Serial.print(F("📲BLE 📫 Sending: "));
    Serial.print(*package);
#endif

    if (deviceConnected) {
    #if DEBUG_MODE && DEBUG_BLE
        Serial.print(F("   BLE device connected. Package: "));
        Serial.println(package->c_str());
        Serial.print(F(" (MTU = "));
        Serial.print(MTU);
        Serial.println(F(")"));
    #endif


    bool isStringNotEmpty = true;
    while (isStringNotEmpty)
      {
        String nextSendMTU = package->substring(0, MTU);
        package->remove(0, MTU);
        isStringNotEmpty = package->length() != 0;

  #if DEBUG_MODE && DEBUG_BLE
        Serial.println(nextSendMTU.c_str());
  #endif

        pCharacteristic->setValue(nextSendMTU.c_str());
        pCharacteristic->notify();
        delay(10);
    }
  #if DEBUG_MODE && DEBUG_BLE
    Serial.println("");
  #endif
  }     
}




void bleInit()
{
  BLEDevice::init(BLE_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_NOTIFY |
          BLECharacteristic::PROPERTY_INDICATE |
          BLECharacteristic::PROPERTY_WRITE_NR);

  pCharacteristic->setCallbacks(new MyBLECallbacks());

  pService->start();
  pServer->getAdvertising()->start();

// Configure advertising data
//   BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
//   pAdvertising->addServiceUUID(SERVICE_UUID);  // Advertise service UUID
//   pAdvertising->setScanResponse(true);         // Enable scan response
//   pAdvertising->setManufacturerData(MANUFACTURER_ID, manufacturerData, sizeof(manufacturerData));  // Optional

  // Set advertising parameters
//   pAdvertising->setMinInterval(0x20);  // 32.5ms (BLE standard)
//   pAdvertising->setMaxInterval(0x40);  // 65ms

  // Start advertising
//   pAdvertising->start();
//   Serial.println("BLE Server Ready! Advertising with custom data...");
}


#endif




