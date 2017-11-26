#include <Arduino.h>

/*
Timer Overflow Interrupt Example

James H. Bannock, 2017
Norges teknisk-naturvitenskapelige universitet (NTNU)

In this example a message is printed on the serial port every 0.5 s

Notes:
  - All timer units equate to the prescaler clock settings of the timer
  - Clock speed is 16 MHz -> 1 cycle takes 62.5 ns
  - Here we use 1/256 prescaling (i.e. the timer increments every 256 cycles)
  - Therefore, 1 timer unit equates to 16 us (62.5 ns * 256)
  - The prescaler is set by enabling the Clock Setting bit CS12 in the Timer/
    Counter Control Register B (TCCR1B)
  - Timer1 is the 16-bit timer (i.e. has a max. value of 65535)
  - Overflow ISR will trigger when Timer Counter 1 (TCNT1) > 65535
  - TCNT1 can be preloaded to vary the delay time before overflow occurs,
    accordingly: delay = 65536 - p, where p is the preloaded timer value
    (overflow triggers on 65535 + 1)
  - TCNT1 must be preloaded in each ISR call, else the delay will equal 65535
  - Here, TIMER_PRELOAD is set to 34286, which corresponds to an ISR call every
    0.5 s (delay = 500 ms / 16 us = 31250, p = 65536 - 31250 = 34286)
  - The ISR is enabled by enabling the Output Compare match A Interrupt Enable
    for Timer 1 (OCIE1A) in the Timer Interrupt Mask (TIMSK1)
*/

#define TIMER_PRELOAD 34286

void setup() {
  Serial.begin(9600); (!Serial); // setup serial port
  setupTimer(); // setup timer
}

ISR(TIMER1_OVF_vect) { // Timer1 overflow (OVF) interrupt service routine (ISR)
  Serial.println("Timer Overflow Interrupt Triggered");
  TCNT1 = TIMER_PRELOAD; // reload timer counter
}

void loop() {}

void setupTimer() {
  noInterrupts(); // disable all interrupts
  TCCR1A = 0; // reset timer/counter control register A for Timer1
  TCCR1B = 0; // reset timer/counter control register B for Timer1

  TCNT1 = TIMER_PRELOAD; // timer/counter
  TCCR1B |= (1 << CS12); // clock select (Timer1, bit2 -> 1/256 prescale)
  TIMSK1 |= (1 << TOIE1); // enable the timer overflow interrupt (TOIE1) in the
                          // timer interrupt mask for Timer1 (TIMSK1)
  interrupts(); // enable all interrupts
}
