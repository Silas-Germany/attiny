#include "bt_class.h"

void printResult(String result) {
  Serial.print("Received data: " + result);
}

BTClass bluetooth("ESP32_BLE_UART_Server", printResult);


void setup() {
  Serial.begin(9600);
  while (!Serial);

  bluetooth.initialize();
  Serial.println("Bluetooth ready");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    String input = Serial.readString();
    bluetooth.write(input);
    Serial.println("Sent data: " + input);
  }
  delay(200);
}