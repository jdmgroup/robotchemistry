/*

  Stepper Motor / Easydriver Demo (Robot Chemistry 4.IS8)

  James H. Bannock and John C. de Mello

  Department of Chemistry,
  Imperial College London

  Version 1.02

  Created: Feb 2015
  Updated: Nov 2016

  ---

  To be used on syringe pump demonstrator without the Tinkerkit shield in place.

  Stepper motor is a McLennan 35911/4413 200-step stepper -> 1600 microsteps/turn

  Component List and Socket Positions:

  Outputs:
  - Direction pin (BLACK WIRE) INTO EASYDRIVER - 2
  - Step pin (RED WIRE) INTO EASYDRIVER - 3

*/

#define DIR_PIN 2
#define STEP_PIN 3

void setup() {
  
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
}

void loop() {

  // rotate a specific number of microsteps

  // speed in revolutions per second
  
  rotate(1600, 1, .5); // clockwise (forward) rotate
  delay(1000);

  rotate(1600, 0, .25); // anticlockwise (reverse) rotate
  delay(1000);
}

void rotate(int steps, int dir, float speed) {

  int stepsPerRevolution = 200; // stepper motor resolution
  
  digitalWrite(DIR_PIN, dir);
  
  float usDelay = (500000 / stepsPerRevolution) / speed;

  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(usDelay);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(usDelay);
  }
}
