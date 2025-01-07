#include "bt_class.h"

BTClass bluetooth("OWN_DEVICE_TEST");

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
    input.trim();
    Serial.println("Sent data: " + input);
  }
  if (bluetooth.available()) {
    String input = bluetooth.readString();
    Serial.println("Received data: " + input);
  }
  delay(200);
}