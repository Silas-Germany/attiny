#ifndef SLCD_H
#define SLCD_H

#include <Arduino.h>

#define SLCD_INIT           0xA3
#define SLCD_INIT_ACK       0xA5
#define SLCD_INIT_DONE      0xAA
#define SLCD_CONTROL_HEADER 0x9F
#define SLCD_CHAR_HEADER    0xFE
#define SLCD_CURSOR_HEADER  0xFF
#define SLCD_CURSOR_ACK     0x5A
#define SLCD_RETURN_HOME    0x61
#define SLCD_DISPLAY_OFF    0x63
#define SLCD_DISPLAY_ON     0x64
#define SLCD_CLEAR_DISPLAY  0x65
#define SLCD_CURSOR_OFF     0x66
#define SLCD_CURSOR_ON      0x67
#define SLCD_BLINK_OFF      0x68
#define SLCD_BLINK_ON       0x69
#define SLCD_SCROLL_LEFT    0x6C
#define SLCD_SCROLL_RIGHT   0x72
#define SLCD_NO_AUTO_SCROLL 0x6A
#define SLCD_AUTO_SCROLL    0x6D
#define SLCD_LEFT_TO_RIGHT  0x70
#define SLCD_RIGHT_TO_LEFT  0x71
#define SLCD_POWER_ON       0x83
#define SLCD_POWER_OFF      0x82
#define SLCD_INVALIDCOMMAND 0x46
#define SLCD_BACKLIGHT_ON   0x81
#define SLCD_BACKLIGHT_OFF  0x80

class SLCD {

private:
  uint8_t _rx;
  uint8_t _tx;

public:
  SLCD(uint8_t rx, uint8_t tx) : _rx(rx), _tx(tx) {};

  void begin() {
    Serial1.begin(9600, SERIAL_8N1, _rx, _tx);
    delay(2);
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_POWER_OFF);
    delay(1);
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_POWER_ON);
    delay(1);
    Serial1.write(SLCD_INIT_ACK);
    while(1) {
      if (Serial1.available() > 0 && Serial1.read() == SLCD_INIT_DONE) break;
    }
    delay(2);
  }

  void clear() {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_CLEAR_DISPLAY);
    delay(4);
  }

  void home() {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_RETURN_HOME);
    delay(4);
  }

  void setCursor(uint8_t column, uint8_t row) {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_CURSOR_HEADER);
    Serial1.write(column);
    Serial1.write(row);
  }

  void noDisplay() {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_DISPLAY_OFF);
  }

  void display() {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_DISPLAY_ON);
  }

  void noCursor() {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_CURSOR_OFF);
  }

  void cursor() {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_CURSOR_ON);
  }

  void noBlink() {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_BLINK_OFF);
  }

  void blink() {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_BLINK_ON);
  }

  void scrollDisplayLeft() {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_SCROLL_LEFT);
  }

  void scrollDisplayRight() {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_SCROLL_RIGHT);
  }

  void leftToRight() {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_LEFT_TO_RIGHT);
  }

  void rightToLeft() {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_RIGHT_TO_LEFT);
  }

  void autoscroll() {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_AUTO_SCROLL);
  }

  void noAutoscroll() {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_NO_AUTO_SCROLL);
  }

  void power() {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_POWER_ON);
  }

  void noPower() {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_POWER_OFF);
  }

  void backlight() {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_BACKLIGHT_ON);
  }

  void noBacklight() {
    Serial1.write(SLCD_CONTROL_HEADER);
    Serial1.write(SLCD_BACKLIGHT_OFF);
  }

  void print(uint8_t b) {
    Serial1.write(SLCD_CHAR_HEADER);
    Serial1.write(b);
  }

  void print(const char b[]) {
    Serial1.write(SLCD_CHAR_HEADER);
    Serial1.write(b);
  }

  void print(const String b) {
    Serial1.write(SLCD_CHAR_HEADER);
    Serial1.write(b.c_str());
  }

  void print(unsigned long n, uint8_t base) {
    Serial1.write(SLCD_CHAR_HEADER);
    Serial1.print(n, base);
  }

  void write(const char b[], uint8_t len) {
    Serial1.write(SLCD_CHAR_HEADER);
    Serial1.write(b, len);
  }
};

#endif