#include <Arduino.h>

/*
External Event Counter Example

James H. Bannock, 2017
Norges teknisk-naturvitenskapelige universitet (NTNU)

In this example a counter is used to count button presses. When 10 presses have
been counted an ISR executes printing a message on the serial port and then the
count is restarted.

Notes:
  - A button is attached to Pin 5 and GND
  - INPUT_PULLUP is used to pin the voltage of the input to 5V while the button
    is open
  - external interrupt with falling-edge detection is configured by enabling CS11
    and CS12 in the Timer/Counter Control Register B for Timer 1 (TCCR1B)
  - Clear Timer on Compare (CTC) mode is used to reset the counter when the ISR
    executes, this is enabled by enabling WGM12 in TCCR1B
  - the ISR is enabled by enabling the Output Compare A match Interrupt Enable
    for Timer 1 (OCIE1A) in the Timer Interrupt Mask (TIMSK1).
*/

#define TIMER1_PIN 5 // TIMER1 pin is fixed in hardware
#define N_BUTTON_PRESSES 10

int8_t counter = -1; // ensures we get a Serial.print for counter = 0

void setup() {
  Serial.begin(9600); (!Serial); // setup the serial port
  pinMode(TIMER1_PIN, INPUT_PULLUP); // pull-up to pin voltage to 5V
  setupCounter(); // abstract the counter setup
}

ISR(TIMER1_COMPA_vect) { // Interrupt Service Routine (ISR) for Output Compare
                         // Register A on Timer 1 (OCR1A)
  Serial.println("10 button presses detected!"); // print a message to port
}

void loop() { // this keeps us up-to-date with the current count, it serves no
              // purpose in the interrupt routine
  if (TCNT1 != counter) { // if the Timer Counter 1 is different from counter
    counter = TCNT1; // update counter
    Serial.print(F("Counter: ")); Serial.println(counter); // print counter value
  }
}

void setupCounter() {
  noInterrupts();
  TCCR1A = 0; TCCR1B = 0; // reset Timer/Counter Control Registers A and B

  // enable external interrupt - falling-edge detection (5V -> GND)
  TCCR1B |= (1 << CS12);
  TCCR1B |= (1 << CS11);

  OCR1A = N_BUTTON_PRESSES; // set the counter limit

  TCNT1 = 0; // reset the counter

  TCCR1B |= (1 << WGM12); // CTC mode - clear the counter when ISR activates
  TIMSK1 |= (1 << OCIE1A); // enable the ISR
  interrupts();
}
