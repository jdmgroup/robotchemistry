#include <Arduino.h>

/*
Combining compare interrupts COMPA and COMPB on Timer1

James H. Bannock
Norges teknisk-naturvitenskapelige universitet (NTNU)

This example makes the LED blink with a frequency of 0.5 Hz

Notes:
  - to use both COMPA and COMPB the CTC (Clear Timer on Compare match) mode
    must not be used, else TCNTx will reset by the first interrupt service
    routine (ISR) to execute
  - here we use x1/1024 prescaling (set Clock Select (CS) bits CS10 and CS12
    high in the Timer/Counter Control Register B (TCCR1B)), and so each timer
    unit equates to 64 us
  - COMPA turn the LED on, COMPB turns the LED off
  - we require a delay of 1 s between the two ISRs, which is equivalent to 15625
    timer units
  - here OCR1B > OCR1A, and so we must reset the counter (TCNT1) in the ISR for
    COMPB
*/

void setup() {
  pinMode(13, OUTPUT); // enable the LED pin as output
  setupTimer(); // abstract timer setup
}

ISR(TIMER1_COMPA_vect) { // ISR for Timer 1 Output Compare Register A (OCR1A)
  digitalWrite(13, HIGH); // turn on the LED
}

ISR(TIMER1_COMPB_vect) { // ISR for Timer 1 Output Compare Register B (OCR1B)
  digitalWrite(13, LOW); // turn off the LED
  TCNT1 = 0; // must manually reset the Timer Counter to zero as CTC mode is disabled
}

void loop() {} // leave blank

void setupTimer() {
  noInterrupts(); // turn off interrupts
  
  TCCR1A = 0; // reset the Timer/Counter Control Register A for Timer 1
  TCCR1B = 0; // reset the Timer/Counter Control Register B for Timer 1
  
  // enable 1/256 prescaling
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  
  // enable COMPA and COMPB ISRs
  TIMSK1 |= (1 << OCIE1A);
  TIMSK1 |= (1 << OCIE1B);
  
  OCR1A = 15625; // set time delay for the COMPA ISR to 1 s
  OCR1B = 15625 * 2; // set time delay for the COMPB ISR to 2s
  
  interrupts(); // turn on interrupts
}
