#ifndef BTClass_H
#define BTClass_H

#include <BLEDevice.h>
#include <BLE2902.h>
#include <Arduino.h>

#define SERVICE_UUID        "d4d42295-0f48-4f4e-9823-4a5773e60b8f"
#define CHARACTERISTIC_UUID "72b73b37-fd9b-44b8-ba41-412566a7c5e4"

typedef std::function<void(const String)> DataCallback;

class BTCallbacks : public BLECharacteristicCallbacks {

private:
  bool _new_data;
  String _data;

public:
  BTCallbacks() : _data("") {}

  void onWrite(BLECharacteristic *pC, esp_ble_gatts_cb_param_t *param) {
    _new_data = true;
    _data = String(pC->getValue().c_str());
  }

  bool available() {
    if (!_new_data) return false;
    _new_data = false;
    return true;
  }

  String readString() {
    return _data;
  }
};

class BTClass {

private:

  BLECharacteristic *_pCharacteristic;
  std::__cxx11::string _name;
  BTCallbacks _reader;

public:

  BTClass(std::__cxx11::string name) : _reader() {
    _name = name;
  }

  void initialize() {
    BLEDevice::init(_name);
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);

    _pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY
                      );
    _pCharacteristic->addDescriptor(new BLE2902());
    _pCharacteristic->setCallbacks(&_reader);

    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
  }

  void write(String data) {
    _pCharacteristic->setValue(data.c_str());
    _pCharacteristic->notify();
  }

  bool available() {
    return _reader.available();
  }

  String readString() {
    return _reader.readString();
  }
};

#endif
