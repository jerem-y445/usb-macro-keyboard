#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

//Button definitions
#define BUTTON1 1
#define BUTTON2 2
#define BUTTON3 3
#define BUTTON4 4
#define NUM_BUTTONS 4

//Profile selection and encoder update variables
int profileSelectIndex = 0;
double encoderUpdate = 0;

//Defining USB HID Keyboard class
USBHIDKeyboard Keyboard;

//Structs for selecting macros within profiles
struct Macro {
  uint8_t modifier;
  char key;
};

struct Profile {
  const char* name;
  Macro macros[NUM_BUTTONS];
};

//Setting profiles with specific macros
Profile profiles[] {
  {"Browser", {
    {KEY_LEFT_CTRL, 't'},
    {KEY_LEFT_CTRL, 'w'},
    {KEY_LEFT_CTRL, 'n'},
    {KEY_LEFT_CTRL, 'f'}
  }},
  {"VSCode", {
    {KEY_LEFT_CTRL, 's'},
    {KEY_LEFT_CTRL, 'a'},
    {KEY_LEFT_CTRL, 'z'},
    {KEY_LEFT_CTRL, 'y'}
  }},
};

//Prototypes
void performMacro(Macro macroSelection);

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
  /*
  if (digitalRead(BUTTON1)) {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('t');
    delay(100);
    Keyboard.releaseAll();
    delay(100); // for debounce delay to prevent multiple presses from being detectedHello from Arduino USB HID!
  }
  */

  // Select profile
  if (encoderUpdate < 10) {
    profileSelectIndex = 0;
  }
  else if (encoderUpdate < 200) {
    profileSelectIndex = 1;
  }
  else if (encoderUpdate < 500) {
    profileSelectIndex = 2;
  }
  else if (encoderUpdate < 1000) {
    profileSelectIndex = 3;
  }

  if (digitalRead(BUTTON1)) {
    int buttonIndex = getButtonIndex(BUTTON1);
    Macro selectedMacro = profiles[profileSelectIndex].macros[buttonIndex];
    performMacro(selectedMacro);
  }
  else if (digitalRead(BUTTON2)) {
    int buttonIndex = getButtonIndex(BUTTON2);
    Macro selectedMacro = profiles[profileSelectIndex].macros[buttonIndex];
    performMacro(selectedMacro);
  }
  else if (digitalRead(BUTTON3)) {
    int buttonIndex = getButtonIndex(BUTTON3);
    Macro selectedMacro = profiles[profileSelectIndex].macros[buttonIndex];
    performMacro(selectedMacro);
  }
  else if (digitalRead(BUTTON4)) {
    int buttonIndex = getButtonIndex(BUTTON4);
    Macro selectedMacro = profiles[profileSelectIndex].macros[buttonIndex];
    performMacro(selectedMacro);
  }
  
}

void performMacro(Macro macroSelection) {
  Keyboard.press(macroSelection.modifier);
  Keyboard.press(macroSelection.key);
  delay(100);
  Keyboard.releaseAll();
  delay(100);
}

int getButtonIndex(int gpioPin) {
  return gpioPin - 1;
}