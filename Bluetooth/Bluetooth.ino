#include "bt_class.h"
#include "slcd.h"
#include <Wire.h>               // Include Wire for I2C communication
#include <MPU6050.h>             // Include MPU6050 library
MPU6050 mpu;                     // Create an MPU6050 object

#define PWM_PIN 9 // Define the pin to control via PWM

BTClass bluetooth("BLE_TEST_DEVICE");
SLCD lcd(5, 6);

void setup() {
  pinMode(PWM_PIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.setTimeout(100);
  bluetooth.initialize();
  Serial.println("Bluetooth ready");
  lcd.begin();
  Wire.begin();                  // Start I2C communication
  mpu.initialize();              // Initialize the MPU6050
  // Check if the MPU6050 is connected
  if (mpu.testConnection()) {
    Serial.println("MPU6050 connected successfully");
  } else {
    Serial.println("MPU6050 connection failed");
    //while (1);  // Stop here if no connection
  }
  mpu.CalibrateAccel();
  mpu.CalibrateGyro();
}

void print(int16_t d) {
  if (d < 0) {
    lcd.print("-");
    lcd.print(d * -1, 10);
  } else {
    lcd.print(d, 10);
  }
  if (d < 2000) Serial.print(d);
  else Serial.print(0);
}

void loop() {
  int16_t ax, ay, az, gx, gy, gz;

  // Get raw data from the MPU6050
  mpu.getAcceleration(&ax, &ay, &az);
  mpu.getRotation(&gx, &gy, &gz);

  // Print raw accelerometer and gyroscope data
  lcd.clear();
  lcd.print(""); print(ax);
  Serial.print("\t");
  lcd.print("|"); print(ay);
  Serial.print("\t");
  lcd.print("|"); print(az);
  Serial.print("\t");

  lcd.setCursor(0, 1);
  lcd.print(""); print(gx);
  Serial.print("\t");
  lcd.print("|"); print(gy);
  Serial.print("\t");
  lcd.print("|"); print(gz);
  Serial.println("");
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    String input = Serial.readString();
    bluetooth.write(input);
    input.trim();
    Serial.println("Sent data: " + input);
    lcd.clear();
    lcd.print("Sent data: " + input);
  }
  if (bluetooth.available()) {
    String input = bluetooth.readString();
    Serial.println("Received data: " + input);

    int pwmValue = input.toInt();
    if (pwmValue >= 0 && pwmValue <= 255) {
      analogWrite(PWM_PIN, pwmValue);   // Set the PWM value
    }
  }
  delay(70);
}