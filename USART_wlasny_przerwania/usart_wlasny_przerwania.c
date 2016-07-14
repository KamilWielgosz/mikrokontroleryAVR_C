#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>



//////////////////////////////////////////////ODEBIRANIE//////////////////////////////
#define UART_RX_BUF_SIZE 32
#define UART_RX_BUF_MASK (UART_RX_BUF_SIZE - 1)

volatile char odebralem = 0;
volatile char UART_RxBuf[UART_RX_BUF_SIZE - 1];
volatile uint8_t UART_RxHead;
volatile uint8_t UART_RxTail;

char uart_getc(void){

	if(UART_RxHead == UART_RxTail) return 0;

	UART_RxTail = (UART_RxTail +1) & UART_RX_BUF_MASK;

	return UART_RxBuf[UART_RxTail];
}

ISR(USART_RXC_vect){
	uint8_t tmp_head;
   odebralem = 1;
	char data;
	data = UDR;

	tmp_head = (UART_RxHead +1) & UART_RX_BUF_MASK;

	if(tmp_head == UART_RxTail){

	} else {

		UART_RxHead = tmp_head;
		UART_RxBuf[tmp_head] = data;
	}
}
//////////////////////////////////////////////////ODEBIRANIE////////////////////////////////////


////////////////////////////////////INICJALZACJA////////////////////////////////////////
void USART_Init(void){

#define BAUD 57600

#include <util/setbaud.h>

UBRRH = UBRRH_VALUE;
UBRRL = UBRRL_VALUE;

#if USE_2X
UCSRA |= (1<<U2X);
#else
UCSRA &= ~(1<<U2X);
#endif

UCSRC |= (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);

UCSRB |= (1<<RXEN) | (1<<TXEN) | (1<<RXCIE);
//RXCIE - USART_RXC_vect - przerwanie po odebraniu bajtu
//UDRIE - USART_UDRE_vect - przerwanie gdy bufor nadawania(UDR) jest pusty
}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////WYSYLANIE////////////////////////////////
#define UART_TX_BUF_SIZE 16
#define UART_TX_BUF_MASK (UART_TX_BUF_SIZE -1)

volatile char UART_TxBuf[UART_TX_BUF_SIZE];
volatile uint8_t UART_TxHead;
volatile uint8_t UART_TxTail;

void uart_putc(char data){

	uint8_t tmp_head;

	tmp_head = (UART_TxHead +1) & UART_TX_BUF_MASK;

	while(tmp_head == UART_TxTail){}

	UART_TxBuf[tmp_head] = data;
	UART_TxHead = tmp_head;

	UCSRB |= (1<<UDRIE);
}

ISR(USART_UDRE_vect){

	if(UART_TxHead != UART_TxTail){

		UART_TxTail = (UART_TxTail +1) & UART_TX_BUF_MASK;
		UDR = UART_TxBuf[UART_TxTail];
	} else {

		UCSRB &= ~(1<<UDRIE);
	}
}

////////////////////////////////////////////////////////WYSYLANIe/////////////////////////////

int main(){

	DDRC |= (1<<PC5);
	PORTC |= (1<<PC5);

	USART_Init();
    sei();


   uart_putc('A');

   char bufor[20];


	while(1){

		if(odebralem) {
			PORTC ^= (1<<PC5);
			odebralem = 0;
            bufor[0] = uart_getc();

            uart_putc(bufor[0]);



		}



	}


	return 0;
}
