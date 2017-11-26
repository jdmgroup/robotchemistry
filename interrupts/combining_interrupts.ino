#include <Arduino.h>
#include <avr/wdt.h> // need for watchdog interrupt
#include <EEPROM.h> // need for EEPROM access

/*
Three interrupt example

James H. Bannock, 2017
Norges teknisk-naturvitenskapelige universitet (NTNU)

This program uses a timer to read the onboard chip temperature every second, if
the temperature is below a limit the watchdog function is reset, else the
watchdog will trigger a reset of the Arduino. The program also includes a serial
interrupt to read in data from the serial port.

*/

#define TEMP_LIMIT 36 // limit to trip watchdog

void setup() {
  Serial.begin(9600); (!Serial); // setup serial port
  pinMode(13,OUTPUT); // enable LED pin as output

  // EEPROM.read(0) stores a value 'E' if the watchdog reset the Arduino
  if (EEPROM.read(0) == 'E') Serial.println("Temperature Limit Exceeded - Reset!");

  // If the chip if hot, wait for it to cool down, use the LED to indicate
  while(getChipTemp() > TEMP_LIMIT) {
    digitalWrite(13, HIGH); delay(25);
    digitalWrite(13, LOW); delay(25);
  }

  // Chip is now cool, write an 'N', if the Arduino is now reset it wont show
  // the error message in Line 30
  EEPROM.write(0,'N');

  setupWatchdog(); // abstract watchdog setup
  setupTimer(); // abstract timer setup
  Serial.println(F("My Instrument")); // shows that instrument is ready
}

void serialEvent() { // serial port interrupt
  switch (Serial.read()) { // read first byte to choose operation
    case 'R': // run
    runExperiment(); // go to runExperiment() function
    break;
  }
}

ISR(TIMER1_COMPA_vect) {
  // toggle LED so we can see the ISR is active
  (digitalRead(13))? digitalWrite(13, LOW) : digitalWrite(13, HIGH);

  // if temperature is less than limit then safe to reset the watchdog
  if (getChipTemp() < TEMP_LIMIT) wdt_reset(); // resets watchdog
}

ISR(WDT_vect) { // Interrupt Service Routine (ISR) for WatchDog Timer (WDT)
  EEPROM.write(0,'E'); // store an 'E' to trigger an error message upon reset
                       // at line 30
}

void loop() {}

void setupWatchdog() {
  cli(); // turn off interrupts
  wdt_reset(); // reset the watchdog before making changes
  WDTCSR |= (1<<WDCE) | (1<<WDE); // needed to allow settings to be changed

  // WDIE = WatchDog Interrupt enable
  // WDE = WatchDog System reset enable
  // WDP = WatchDog Prescaler (configure these for different timings)
  // combination of WDP0, WDP1 and WDP2 enabled gives a timer of 2 s
  WDTCSR = (1<<WDIE) | (1<<WDE) | (0<<WDP3) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);
  sei(); // turn on interrupts
}

void setupTimer() {
  noInterrupts(); // turn off interrupts
  TCCR1A = 0; // reset Timer/Counter Control Register A for Timer 1
  TCCR1B = 0; // reset Timer/Counter Control Register B for Timer 1

  OCR1A = 15625; // set a timer value equivalent to 1 s

  // enable 1/1024 prescaling
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);

  TCCR1B |= (1 << WGM12); // enable Clear Timer on Compare (CTC) mode
  TIMSK1 |= (1 << OCIE1A); // enable ISR
  interrupts(); // turn on interrupts
}

void runExperiment() { // dummy experiment
  for (int process = 0; process < 100; process++) { // 100 repeats
    Serial.print("Process "); Serial.println(process); // print proces number
    // print chip temperature so we can see it increase
    Serial.print("Current Chip Temp: "); Serial.println(getChipTemp());
    delay(1000); // delay to simulate other functions
  }
}

float getChipTemp() {
  // code required to acquire chip temperature from Atmel 328
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3)); // setup MUX for pin A8
  ADCSRA |= _BV(ADEN); // enable ADC enable (ADEN) in ADC Control and Status
                       // Register A (ADCSRA)
  delay(20);

  ADCSRA |= _BV(ADSC); // start ADC conversion

  while (bit_is_set(ADCSRA,ADSC)); // wait fo ADSC to go low, indicating end of
                                   // ADC conversion
  return ((ADCW - 324.31 ) / 1.22); // converts ADC value to temperature
}
