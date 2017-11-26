#include <Arduino.h>

/*
Using attachInterrupt()

James H. Bannock
Norges teknisk-naturvitenskapelige universitet (NTNU)

This example toggles the LED with every press of a button

Notes:
  - connect a push button switch to PIN 2
  - we are focusing on falling-edge detection (5V -> GND), the input is normally
    high and will be pulled low when the button is pressed, therefore we are
    detecting the moment the switch is pressed (as opposed to the rising-edge
    which will detect GND -> 5V transition)
  - attachInterrupt requires a function name to be provided to act as the
    interrupt function, here it is called toggleLed()
  - digitalPinToInterrupt(pin) is required to convert physical pin number to
    lower-level registry values

*/

#define LED_PIN 13
#define INTERRUPT_PIN 2

bool ledState = LOW;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(INTERRUPT_PIN, INPUT_PULLUP); // pull-up to pin voltage to 5V
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), toggleLed, FALLING);
}

void loop() {}

void toggleLed() {
  ledState = !ledState; // ! means not, here ledState is flipped
  digitalWrite(LED_PIN, ledState);
}
