#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

#define BUTTON1 1
#define BUTTON2 2
#define BUTTON3 3
#define BUTTON4 4

USBHIDKeyboard Keyboard;

void setup() {
  USB.begin();
  Keyboard.begin();
  delay(2000);
  Keyboard.println("Hello from Arduino USB HID!");

  pinMode(BUTTON1, INPUT_PULLDOWN);
  pinMode(BUTTON2, INPUT_PULLDOWN);
  pinMode(BUTTON3, INPUT_PULLDOWN);
  pinMode(BUTTON4, INPUT_PULLDOWN);
}

void loop() {
  if (digitalRead(BUTTON1)) {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('t');
    delay(100);
    Keyboard.releaseAll();
    delay(500); // for debounce delay to prevent multiple presses from being detectedHello from Arduino USB HID!
    
  }
}