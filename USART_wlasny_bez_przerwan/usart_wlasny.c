#include <util/delay.h>
#include <avr/io.h>

void USART_Init(void){

#define BAUD 57600
#include <util/setbaud.h>
	 UBRRH = UBRRH_VALUE;
	 UBRRL = UBRRL_VALUE;

#if USE_2X
  UCSRA |=  (1<<U2X);
#else
  UCSRA &= ~(1<<U2X);
#endif


UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);

UCSRB = (1<<RXEN) | (1<<TXEN);

}

void USART_Transmit(unsigned char data){
	//Czekaj az zwolni sie bufor nadajnika
	//UDRE - flaga ustawiona gdy pobierzemy dane z UDR
	while(!(UCSRA & (1<<UDRE)));

	UDR = data;
}

unsigned char USART_Receive(void){

	while(!(UCSRA & (1<<RXC)));

	return UDR;
}


int main(){

	USART_Init();

	DDRB |= (1<<PC5);
	PORTB |= (1<<PC5);


   unsigned char receiv;

   unsigned char wyslij = 'A';

	while(1){
         receiv =  USART_Receive();

         USART_Transmit(wyslij);

	}




	return 0;
}
