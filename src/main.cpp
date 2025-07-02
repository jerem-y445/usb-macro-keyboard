#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "ezButton.h"

//Button definitions
#define BUTTON1 1
#define BUTTON2 2
#define BUTTON3 3
#define BUTTON4 4
#define NUM_BUTTONS 4

#define CLK_PIN 2
#define DT_PIN 3
#define SW_PIN 4
#define CW_DIRECTION 0
#define CWW_DIRECTION 1

//Profile selection and encoder update variables
int profileSelectIndex = 0;
int direction = CW_DIRECTION;
int CLK_state;
int prev_CLK_state;

//Create an ezButton object assigned to pin 4
ezButton button(SW_PIN);

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

//Setting profiles with application-specific macros
Profile profiles[] {
  {"Browser", { {KEY_LEFT_CTRL, 't'}, {KEY_LEFT_CTRL, 'w'}, {KEY_LEFT_CTRL, 'n'}, {KEY_LEFT_CTRL, 'f'} } },
  {"VSCode", {{KEY_LEFT_CTRL, 's'}, {KEY_LEFT_CTRL, 'a'}, {KEY_LEFT_CTRL, 'z'}, {KEY_LEFT_CTRL, 'y'} } }
};

//Prototypes
void performMacro(Macro macroSelection);
int getButtonIndex(int gpioPin);

void setup() {
  Serial.begin(9600);
  USB.begin();
  Keyboard.begin();
  delay(2000);

  pinMode(BUTTON1, INPUT_PULLDOWN);
  pinMode(BUTTON2, INPUT_PULLDOWN);
  pinMode(BUTTON3, INPUT_PULLDOWN);
  pinMode(BUTTON4, INPUT_PULLDOWN);
  pinMode(CLK_PIN, INPUT);
  pinMode(DT_PIN, INPUT);

  button.setDebounceTime(50);
}

void loop() {
  button.loop(); //must be present to continuously read button state
  
  // Read from clock state
  CLK_state = digitalRead(CLK_PIN);

  // Select profile
  if (CLK_state != prev_CLK_state && CLK_state == HIGH) {
    if (DT_PIN == HIGH) { // in counter-clockwise direction
      if (profileSelectIndex > 0) {
        profileSelectIndex--;
      }
      direction = CWW_DIRECTION;
    }
    else {
      if (profileSelectIndex < 3) { // in clockwise direction
        profileSelectIndex++;
      }
      direction = CW_DIRECTION;
    }
  }

  prev_CLK_state = CLK_state; // record previous clock state for next loop

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

// Performs a keyboard macro depending on profile selected
void performMacro(Macro macroSelection) {
  Keyboard.press(macroSelection.modifier);
  Keyboard.press(macroSelection.key);
  delay(100);
  Keyboard.releaseAll();
  delay(100);
}

// Gets button index relative to the GPIO pin connected to the button pressed
int getButtonIndex(int gpioPin) {
  return gpioPin - 1;
}