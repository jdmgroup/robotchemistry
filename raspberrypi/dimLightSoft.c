#include <wiringPi.h>
#include <softPwm.h>

void main()
{
	wiringPiSetup();
	int pin = 0; // use GPIO0
	softPwmCreate(pin, 0, 100);
	int i;

	for(i = 0; i < 100; i++)
	{
		// breathe in
		softPwmWrite(pin, i);
		delay(15);
	}

	for(i = 99; i >= 0; i--)
	{
		//breathe out
		softPwmWrite(pin, i);
		delay(15);
	}
}
