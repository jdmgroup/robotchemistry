#include <Arduino.h>

#define INTERRUPT_PIN 9

/*
Pin Change Interrupt Example

James H. Bannock, 2017
Norges teknisk-naturvitenskapelige universitet (NTNU)

In this example a message is written to the serial port each time a state change
on a button is detected (rising- and falling-edge detection).

Notes:
  - A button is attached to Pin 9 and GND
  - INPUT_PULLUP is used to pin the voltage of the input to 5V while the Button
    is open
  - On the Uno there are three ports, Pin 9 is on PORT B (PORTB)
  - There are Pin Change Interrupts associated to each port, PCINT0 is for PORTB
  - PCINT0 must be activated by enabling Pin Change Interrupt Enable 0 (PCIE0)
    in the Pin Change Interrupt Control Register (PCICR)
  - Pin 9 corresponds to Pin Change Interrupt 1 (PCINT1) and must be enabled in
    the Pin Change Mask 0 (PCMSK0)
*/

void setup() {
  Serial.begin(9600); (!Serial); // setup the serial port
  pinMode(INTERRUPT_PIN, INPUT_PULLUP); // pull-up to pin voltage to 5V
  setupPinInterrupt(); // abstract pin interrupt setup
}

ISR (PCINT0_vect) { // Interrupt Service Routine (ISR) for Pin Change Interrupt
                    // Request 0 (PCINT0)
  Serial.println("Button State Change Detected!"); // print an output
}

void loop() {}

void setupPinInterrupt() {
  PCICR |= (1 << PCIE0); // enable PCINT0 ISR
  PCMSK0 |= (1 << PCINT1); // enable interrupt on PCINT1 (Pin 9)
}
