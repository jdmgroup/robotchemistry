/*

  Quadrature Encoder Direction Calculation (Robot Chemistry 4.IS8)

  James H. Bannock and John C. de Mello

  Department of Chemistry,
  Imperial College London

  Version 1.02

  Created: Feb 2014
  Updated: Nov 2016

  ---
  
  Requirements:

  Stepper motor driven from an Easydriver.
  
  Two channel quadrature encoder optical encoder coupled to motor shaft.

  Refer to slide 66 in 2016 notes for formulation of matrix

  ---

  Outputs:
  - Direction pin of Easydriver - PIN 2
  - Step pin of Easydriver - PIN 3
  - Channel A of encoder - PIN 5
  - Channel B of encoder - PIN 6

*/

// macro definitions
#define DIR_PIN 2
#define STEP_PIN 3
#define ENCODER_A 5
#define ENCODER_B 6

int eMatrix[4][4] = 
{
  {0, -1, 1, 2},
  {1, 0, 2, -1},
  { -1, 2, 0, 1},
  {2, 1, -1, 0}
};

int OldValue = -2;
int CurrentValue = -1;
int Direction;

// set the motor direction here
int dir = 1; // 1 for clockwise / -1 for anticlockwise

void setup() {

  // turn on serial communication
  Serial.begin(9600);

  // setup pins
  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
}

void loop() {

  OldValue = digitalRead(ENCODER_A) * 2 + digitalRead(ENCODER_B);

  while (CurrentValue != OldValue) {

    rotate(dir, 1000);

    CurrentValue = digitalRead(ENCODER_A) * 2 + digitalRead(ENCODER_B);
    Direction = eMatrix[OldValue][CurrentValue];

    // switch-case to handle different outcomes (alternative is to use nested if-statments)
    switch (Direction) {

      case 2:
        //Serial.println("Unknown");
        break;

      case 1:
        Serial.println("Forward");
        break;

      case -1:
        Serial.println("Reverse");
        break;

      case 0:
        //Serial.print("No Movement");
        break;

      /*
       * Notes: 
       * 
       * In the switch-case statement the use of 'break' causes the switch-case to finish at this point.
       * 
       * Without a break, the code continues to test the argument (Direction) against the remaining cases.
       * 
       */

    }

    OldValue = CurrentValue;

  }

  rotate(dir, 1000);
  
}

void rotate(int microsteps, float delay_) { // function to perform rotation

  if (microsteps > 0) {
    digitalWrite(DIR_PIN, HIGH); // if the number of steps is positive -> motor moves fowards
  }
  else {
    digitalWrite(DIR_PIN, LOW); // if the number of steps is negative -> motor moves in reverse
  }

  for (int i = 0; i < abs(microsteps); i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(delay_); // wait for a defined time (see calc_delay())
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(delay_); // wait for a defined time (see calc_delay())
  }
}
