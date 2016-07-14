#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	//1 - wyjscie 
	//0 - wejscie
	DDRB=0xff;
	/* Replace with your application code */
	while (1)
	{
		PORTB=255;
		_delay_ms(1000);
		PORTB=0;
		_delay_ms(1000);
	}
}
