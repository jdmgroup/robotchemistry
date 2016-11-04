#include <wiringPi.h>

void main()
{
	wiringPiSetup();
	pinMode(0, OUTPUT);
	pinMode(1, INPUT);
	int i;

	if(digitalRead(1) == 1)
	{
		for(i = 0; i < 5 ; i++)
		{
			digitalWrite(0, HIGH);
			delay(500);
			digitalWrite(0, LOW);
			delay(500);
		}
	}
}
