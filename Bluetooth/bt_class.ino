/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include <BLEDevice.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

class BTCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        // Print the received value on the Serial Monitor
        std::string value = pCharacteristic->getValue();
        Serial.print("Received data: ");
        for (int i = 0; i < value.length(); i++) {
            Serial.print(value[i]);
        }
        Serial.println();
    }
};

class BTClass {
  private:
    BLECharacteristic *pCharacteristicRx;
    BLECharacteristic *pCharacteristicTx;
    std::__cxx11::string _name;

  public:
  BTClass(std::__cxx11::string name) {
    _name = name;
  }
  void initialize() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println("Starting BLE work!");

    BLEDevice::init(_name);
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);

    pCharacteristicRx = pService->createCharacteristic(
                                          CHARACTERISTIC_UUID,
                                          BLECharacteristic::PROPERTY_WRITE
                                        );
    pCharacteristicRx->setCallbacks(new BTCallbacks());

    // Create the TX characteristic (for sending data to client)
    pCharacteristicTx = pService->createCharacteristic(
                          CHARACTERISTIC_UUID,
                          BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY 
                        );
    BLEDescriptor *pCCCD = new BLEDescriptor(BLEUUID((uint16_t)0x2902));  // 0x2902 is the UUID for CCCD
    pCCCD->setValue("0x00");  // Set the default value to 0x00 (disabled notifications)
    pCharacteristicTx->addDescriptor(pCCCD);  // Add the CCCD to TX characteristic

    pService->start();
    // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("Characteristic defined! Now you can read it in your phone!");
  }

  void write(String data) {
        // If the data is not empty, send it to the Bluetooth client
        if (data.length() > 0) {
            Serial.print("Sending over Bluetooth: ");
            Serial.println(data);

            // Send the data to the TX characteristic
            pCharacteristicTx->setValue(data.c_str());
            pCharacteristicTx->notify();  // Notify the Bluetooth client with the data
        }
    }
};

BTClass bluetooth("ESP32_BLE_UART_Server");
#define PWM_PIN 9 // Define the pin to control via PWM

void setup() {
  pinMode(PWM_PIN, OUTPUT); // Set the PWM pin as output
  bluetooth.initialize();
}

void loop() {
  // put your main code here, to run repeatedly:
    if (Serial.available()) {
      bluetooth.write(Serial.readString());
    }
  delay(200);
}