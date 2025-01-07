#include <BLEDevice.h>
#include <BLE2902.h>
#include <Arduino.h>

#define SERVICE_UUID        "d4d42295-0f48-4f4e-9823-4a5773e60b8f"
#define CHARACTERISTIC_UUID "72b73b37-fd9b-44b8-ba41-412566a7c5e4"


class BTClass {

private:

    BLECharacteristic *pCharacteristic;
    std::__cxx11::string _name;

    class BTCallbacks: public BLECharacteristicCallbacks {
      void onWrite(BLECharacteristic *pC) {
        // Print the received value on the Serial Monitor
        std::string value = pC->getValue();
        Serial.print("Received data: ");
        for (int i = 0; i < value.length(); i++) {
            Serial.print(value[i]);
        }
        Serial.println();
      }
    };

public:

  BTClass(std::__cxx11::string name) {
    _name = name;
  }

  void initialize() {
    BLEDevice::init(_name);
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY
                      );
    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setCallbacks(new BTCallbacks());

    pService->start();
    // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
  }

  void write(String data) {
    pCharacteristic->setValue(data.c_str());
    pCharacteristic->notify();
  }
};
