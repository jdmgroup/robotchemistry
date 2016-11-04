/*

  Encoder 1/4 Turn Demo (Robot Chemistry 4.IS8)

  James H. Bannock and John C. de Mello

  Department of Chemistry,
  Imperial College London

  Version 1.02

  Created: Feb 2015
  Updated: Nov 2016

  Revisions: Increased commnenting and improved setup

  ---

  Requirements:
  
  Optical quadrature encoder with resolution of 500 counts per revolution attached to Arduino Pin 5.

  ---

  Timer Notes:

     Each timer has two 8-bit registers TCCRxA and TCCRxB which store the configurable setup. (TCCRnX = Timer/Counter Control Register X for Timer n)).

     (All the following terms (such as TCCR1A etc.) are inbuilt macros (like the #define operation) and represent a number - either a register value or a counter value).
     
     Arduinos support multiple timers/counters with varying resolutions.

     The Uno has three timers (Timer0, Timer1 and Timer2).

     Timer1 is a 16 bit timer (i.e. can support a counter value between 0 and 65535 (2^16-1)).

     (Timer0 and Timer2 are 8-bit timers and therefore only support a max. counter value of 255).

     TCNT1 (Timer/Counter Register for Timer1) stores the current value of the counter.

     Here we use a simple implementation of the counter where TCNT1 stores the number of signals received from a quadrature optical encoder.

     To achieve this we set the timer/counter to a configuration where the timer/counter source is an external pin (the encoder), which is fixed on Pin5 for Timer1 on the Uno.

     Note that because we are using the hardware timer we do not need to write: pinMode(5, INPUT_PULLUP);

     The necessary configuration bits we need to setup the counter are in the TCCR1B register.

     We must set the Clock Select (CS) bits to either:

     - Falling-edge detection (5->0V): CS12 = CS11 = 1, CS10 = 0; or
     - Rising-edge detection (0->5V): CS12 = CS11 = CS10 = 1

     When TCNT1 reaches a value of 125 (=1/4 turn on the motor for a 500 count encoder) an if-statement causes the LED on the Arduino to flash and we reset TCNT1 to 0.

     Note that this implementation avoids using an interrupt service routine (ISR) -> see equivalent example for interrupt version
*/

void setup() {

  //Serial.begin(9600);

  pinMode(13, OUTPUT); // activate the LED pin

  // switch off interrupts to avoid conflict while changing settings
  noInterrupts();

  // reset timer/counter registers A and B.
  TCCR1A = 0;
  TCCR1B = 0;

  // set the counter to external Timer1 source Pin5 and configure for rising-edge detection.
  TCCR1B |= (1 << CS12); // set CS12 HIGH - bit 2 (4) - equally you could write TCCR1B |= 4;
  TCCR1B |= (1 << CS11); // set CS11 HIGH - bit 1 (2)
  TCCR1B |= (1 << CS10); // set CS10 HIGH - bit 0 (1)

  //Serial.print("TCCR1B (in decimal): ");
  //Serial.println(TCCR1B);
  //Serial.print("TCCR1B (in binary): ");
  //Serial.println(TCCR1B, BIN);

  //TCCR1B &= ~(1 << CS12);

  //Serial.print("TCCR1B: ");
  //Serial.println(TCCR1B);

  /*
     Nptes:

     X |= Y is shorthand for X = X OR Y (in binary this is effectively an addition operation).

     e.g. suppose X = 2 (B00000010) and Y = 4 (B00000100) then X |= Y would set X = B00000110 => 6

     Y |= (1 << X) is shorthand for setting bit X in byte Y to a value of 1, and equates to a decimal addition of 2^X.

     e.g. take line 73, CS12 = 2 so if Y = 0 intially then the operation has the effect of setting bit 2 to 1, which equates to a decimal value of 4 (2^2)

     Note that if you want to set bit X in byte Y to zero then the OR statement may not work if bit X is set at 1. 
     
     (If you want to set bit X to zero then you can use: Y &= ~(1 << X), where & is an AND operation and ~ is a NOT operation, see commented limes 82-85)
     
     If you uncomment the Serial commands on lines 61 and 77-80 and upload to an Arduino you will see that TCCR1B = 7.

     This is because we sets bits 0 (value 2^0 = 1), 1 (value 2^1 = 2) and 2 (value 2^2 = 4) to 1, which sums to 7.
  */

  // switch on interrupts again
  interrupts();
}

void loop() {

  if (TCNT1 > 125) { // if counter has counted greater than 125 pulses
    digitalWrite(13, HIGH); // LED on
    delay(100); // wait a bit
    digitalWrite(13, LOW); // LED off
    TCNT1 = 0; // reset counter
  }

  delay(10); // wait a bit
}
