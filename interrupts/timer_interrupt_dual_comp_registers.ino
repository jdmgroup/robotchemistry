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
  pinMode(13, OUTPUT);
  setupTimer();
}

ISR(TIMER1_COMPA_vect) {
  digitalWrite(13, HIGH);
}

ISR(TIMER1_COMPB_vect) {
  digitalWrite(13, LOW);
  TCNT1 = 0; //
}

void loop() {}

void setupTimer() {
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = 15625;
  OCR1B = 15625 * 2;
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  TIMSK1 |= (1 << OCIE1A);
  TIMSK1 |= (1 << OCIE1B);
  interrupts();
}
