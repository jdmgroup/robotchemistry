#include <wiringPi.h>

void main()
{
	wiringPiSetup();
	// GPIO1 used for hardware PWM
	int pin = 1;
	// Configure GPIO1 for PWM
	pinMode(pin, PWM_OUTPUT);
	int i;

	for(i = 0; i < 200; i++)
	{
		// breathe in
		pwmWrite(pin, i);
		delay(15);
	}

	for(i = 199; i >= 0; i--)
	{
		//breathe out
		pwmWrite(pin, i);
		delay(15);
	}
}
