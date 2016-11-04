#include <wiringPi.h>

void main()
{
	wiringPiSetup();
	pinMode(0, OUTPUT);

	int i;
	for(i = 0; i < 5 ; i++)
	{
		digitalWrite(0, HIGH);
		delay(500);
		digitalWrite(0, LOW);
		delay(500);
	}
}
