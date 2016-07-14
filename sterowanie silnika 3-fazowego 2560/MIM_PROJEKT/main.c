
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
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

ISR(USART0_RX_vect){

	uint8_t tmp_head;
	odebralem = 1;
	char data;
	data = UDR0;

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

	#define BAUD 2400

	#include <util/setbaud.h>

	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;


	UCSR0C |= (1<< UMSEL00) | (1<<UCSZ01) | (1<<UCSZ00);

	UCSR0B |= (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
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

	UCSR0B |= (1<<UDRIE0);
}

/*
ISR(USART0_UDRE_vect){

	if(UART_TxHead != UART_TxTail){

		UART_TxTail = (UART_TxTail +1) & UART_TX_BUF_MASK;
		UDR0 = UART_TxBuf[UART_TxTail];
		} else {

		UCSR0B &= ~(1<<UDRIE0);
	}
}
*/

#define RS_LCD 2
#define EN_LCD 3
#define PORT_LCD PORTF
#define KONF_5 PF5
#define KONF_4 PF4

#define PRZYCISK1 (1<<PK7)
#define PRZYCISK2 (1<<PK6) 
#define PRZYCISK_STOP (1<<PK3)
#define WCISNIETY_STOP !(PINK & PRZYCISK_STOP)
#define WCISNIETY1 !(PINK & PRZYCISK1)
#define WCISNIETY2 !(PINK & PRZYCISK2)

#define PRZYCISK_CZ1 (1<<PK2)
#define PRZYCISK_CZ2 (1<<PK1)
#define WCISNIETY_CZ1 !(PINK & PRZYCISK_CZ1)
#define WCISNIETY_CZ2 !(PINK & PRZYCISK_CZ2)

#define PRZYCISK_KAT1 (1<<PL7)
#define PRZYCISK_KAT2 (1<<PL5)
#define PRZYCISK_START_KAT (1<<PL3)

#define WCISNIETY_KAT1 !(PINL & PRZYCISK_KAT1)
#define WCISNIETY_KAT2 !(PINL & PRZYCISK_KAT2)
#define WCISNIETY_START_KAT !(PINL & PRZYCISK_START_KAT)


#define KIER_PRAWO (1<<PK5)
#define KIER_LEWO  (1<<PK4)

void wlacz_LCD(void);
void czysc_LCD(void);
void wyslij_do_LCD(uint8_t bajt);
void wyswietl_napis(char *napis, uint8_t dlugosc);
void linijka(uint8_t gora_dol);


	volatile int8_t flaga_kat = 0;
	volatile int8_t prawo = 0;
	volatile int8_t lewo = 0;

int main(){
	
	USART_Init();
	//char bufor[20];

	uint16_t licznik_kat = 0;
	
	uint8_t licznik_predkosci = 0;

    char tab_znak_char [11];
	tab_znak_char[0] = '0';
	tab_znak_char[1] = '1';
	tab_znak_char[2] = '2';
	tab_znak_char[3] = '3';
	tab_znak_char[4] = '4';
	tab_znak_char[5] = '5';
	tab_znak_char[6] = '6';
	tab_znak_char[7] = '7';
	tab_znak_char[8] = '8';
	tab_znak_char[9] = '9';
	
	uint16_t tab_znak_int [11];
	tab_znak_int[0] = 10000;
	tab_znak_int[1] = 8000;
	tab_znak_int[2] = 6000;
	tab_znak_int[3] = 5000;
	tab_znak_int[4] = 4000;
	tab_znak_int[5] = 3000;
	tab_znak_int[6] = 2000;
	tab_znak_int[7] = 1000;
	tab_znak_int[8] = 800;
	tab_znak_int[9] = 400;



	
    char napis[17];
	napis[0] = 'K';
	napis[1] = 'I';
	napis[2] = 'E';
	napis[3] = 'R';
	napis[4] = 'U';
	napis[5] = 'N';
	napis[6] = 'E';
	napis[7] = 'K';
	napis[8] = ':';
	napis[9] = ' ';
	napis[10] = ' ';
	napis[11] = ' ';
	napis[12] = ' ';
	napis[13] = ' ';
	napis[14] = ' ';
	napis[15] = ' ';
	
	char napis2[17];
	napis2[0] = 'P';
	napis2[1] = 'R';
	napis2[2] = 'E';
	napis2[3] = 'D';
	napis2[4] = 'K';
	napis2[5] = ':';
	napis2[6] = ' ';
	napis2[7] = ' ';
	napis2[8] = ' ';
	napis2[9] = 'K';
	napis2[10] = 'A';
	napis2[11] = 'T';
	napis2[12] = ':';
	napis2[13] = ' ';
	napis2[14] = ' ';
	napis2[15] = ' ';
		
	wlacz_LCD();

	

	wyswietl_napis(napis, 11);
	
	PORT_LCD &= ~(1<<RS_LCD);
	wyslij_do_LCD(0xC0);
	PORT_LCD |= (1<<RS_LCD);
	_delay_us(40);
	wyswietl_napis(napis2, 16);
	

	DDRB |=0xff;
	DDRK |= KIER_LEWO | KIER_PRAWO;
	DDRK &= ~(PRZYCISK1 | PRZYCISK2 | PRZYCISK_STOP | PRZYCISK_CZ1 | PRZYCISK_CZ2);
	
	PORTK |= PRZYCISK1 | PRZYCISK2 | KIER_PRAWO | KIER_LEWO | PRZYCISK_STOP | PRZYCISK_CZ1 | PRZYCISK_CZ2;
	
	
	DDRL &= ~(PRZYCISK_KAT1 | PRZYCISK_KAT2 | PRZYCISK_START_KAT);
	PORTL |= PRZYCISK_KAT1 | PRZYCISK_KAT2 | PRZYCISK_START_KAT;
	
   //Inicjalizacja Timer1 (PWM)
   TCCR1A |= (1<<COM1A1) //Zmiana stanu wyjœcia OC1A na niski przy porównaniu A
   |  (1<<COM1B1) //Zmiana stanu wyjœcia OC1B na niski przy porównaniu B
   |  (1<<WGM11); //Tryb 14 (FAST PWM, TOP=ICR1)

   TCCR1B |= (1<<WGM13) | (1<<WGM12)  //Tryb 14 (FAST PWM, TOP=ICR1)
   | (1<<CS10);              //brak preskalera
   
   	OCR1A=20;
   	OCR1B=20;
   
   			ICR1 = 10000; //zakres 0 - 65535
   
   //Inicjalizacja Timer3 (CTC)
  
/*
  TCCR3B |= (1<<WGM32) | (1<<CS30) | (1<<CS31);
  TIMSK3 |= (1<<OCIE3A);
  
  OCR3A = 5000;
  TCNT3 = 0;
  
  */
/*
  TCCR2B |= (1<<CS01);    // Ÿród³em CLK, preskaler 8
  TIMSK2 |= (1<<TOIE2);   //Przerwanie overflow (przepe³nienie timera)

*/


    sei();             // enable all interrupts 
   
   

	


	while(1){
		
		uart_putc('A');
		
		if(odebralem) {
			//		PORTC ^= (1<<PC5);
			PORTB ^= (1<<PB1);
			odebralem = 0;
		//	bufor[0] = uart_getc();

			//      uart_putc(bufor[0]);



		}
		
		 if(WCISNIETY1){

			 _delay_ms(80);
			 if(WCISNIETY1){
				 
				  	linijka(0);
				 			 	
				 	napis[10] = 'P';
				 	napis[11] = 'R';
				 	napis[12] = 'A';
				 	napis[13] = 'W';
				 	napis[14] = 'O';
					 
				 	wyswietl_napis(napis, 16);
				 
				    // TCNT3 = 0;
				     PORTK |= KIER_LEWO;
                     PORTK &= ~KIER_PRAWO;
                     prawo = 1;

			 }

		 }

		 if(WCISNIETY2){

			 _delay_ms(80);
			 if(WCISNIETY2){
				 
				 	 linijka(0);
				 	 		 	 
				 	 napis[10] = 'L';
				 	 napis[11] = 'E';
				 	 napis[12] = 'W';
				 	 napis[13] = 'O';
				 	 napis[14] = ' ';
				 	 
				 	 wyswietl_napis(napis, 16);
					  //  TCNT3 = 0;
				 		PORTK |= KIER_PRAWO;
				 		PORTK &= ~KIER_LEWO;
						 lewo = 1;

			 }

		 }
		 
		 
		 if(WCISNIETY_STOP){
			 
			 _delay_ms(80);
			 
			 if(WCISNIETY_STOP){
				 
				 linijka(0);
				 				 	 
				 napis[10] = 'S';
				 napis[11] = 'T';
				 napis[12] = 'O';
				 napis[13] = 'P';
				 napis[14] = ' ';
				 
				 wyswietl_napis(napis, 16);
				 
				 PORTK |= KIER_LEWO | KIER_PRAWO;
				 prawo = 0;
				 lewo = 0;
				 
			 }
			 
			 
		 }
		 
		 
		 if(WCISNIETY_CZ1){
			 
			 _delay_ms(80);
			 
			 if(WCISNIETY_CZ1){
				 
				if(licznik_predkosci > 0){
				  licznik_predkosci--;
				  
				  
				  napis2[6] = tab_znak_char[licznik_predkosci];
				  
				  linijka(1);
				  wyswietl_napis(napis2, 16);
				  
				  ICR1 = tab_znak_int[licznik_predkosci];
				}
				 
			 }
			 
			 
		 }
		 
		   if(WCISNIETY_CZ2){
			 		 
			  _delay_ms(80);
			 		 
			  if(WCISNIETY_CZ2){
				  
				  if(licznik_predkosci < 9){
				  licznik_predkosci++;
				  
				  
				  napis2[6] = tab_znak_char[licznik_predkosci];
				  
				  linijka(1);
				  wyswietl_napis(napis2, 16);
				  
				  ICR1 = tab_znak_int[licznik_predkosci];
				  }
				 		 
			  }
			 		 
			 		 
		 		 }
				  
				  
			uint16_t zapisz_int = 0;	  
			uint16_t moje_int = 0;
				  
			if(WCISNIETY_KAT1){
				
				
				_delay_ms(80);
			   if(WCISNIETY_KAT1){
				   
				   if(licznik_kat < 359){
				   licznik_kat++;
				   OCR3A = (4800/360)*licznik_kat;
				   
				   moje_int = licznik_kat;
				   
				   if(moje_int < 10){
					   
					   zapisz_int = moje_int % 10;	
					   napis2[13] = zapisz_int + '0';			   
					   
				   } else if(moje_int < 100){
					   
					   zapisz_int = moje_int % 10;
					   napis2[14] = zapisz_int + '0';
					   moje_int = moje_int / 10;
					   zapisz_int = moje_int % 10;
					   napis2[13] = zapisz_int + '0';
					   
				   } else  if(moje_int <= 360){
					   
					   zapisz_int = moje_int % 10;
					   napis2[15] = zapisz_int + '0';
					   moje_int = moje_int / 10;
					   zapisz_int = moje_int % 10;
					   napis2[14] = zapisz_int + '0';
					   moje_int = moje_int / 10;
					   zapisz_int = moje_int % 10;
					   napis2[13] = zapisz_int + '0';
					   
				   }
				   
				   
				   linijka(1);
				   wyswietl_napis(napis2, 16);
				   
				   
				   }
				   
			   }
				
				
			}
				  
			
			if(WCISNIETY_KAT2){
			
	_delay_ms(80);
	   if(WCISNIETY_KAT2){
				
		   if(licznik_kat > 0){
					   
		     licznik_kat--;
			 OCR3A = (4800/360)*licznik_kat;
					   
			 	 moje_int = licznik_kat;
					   				   
			 if(moje_int < 10){
						   				   
			 zapisz_int = moje_int % 10;
			 napis2[13] = zapisz_int + '0';
						   				   
			 } else if(moje_int < 100){
						   				   
			 zapisz_int = moje_int % 10;
			 napis2[14] = zapisz_int + '0';
			 moje_int = moje_int / 10;
			 zapisz_int = moje_int % 10;
			 napis2[13] = zapisz_int + '0';
						   				   
			 } else if(moje_int <= 360){
						   				   
			  zapisz_int = moje_int % 10;
			  napis2[15] = zapisz_int + '0';
		      moje_int = moje_int / 10;
			  zapisz_int = moje_int % 10;
		      napis2[14] = zapisz_int + '0';
			  moje_int = moje_int / 10;
			  zapisz_int = moje_int % 10;
			  napis2[13] = zapisz_int + '0';
						   				   
					   	 }
										  
										  				   
          linijka(1);
          wyswietl_napis(napis2, 16);
					   
				   }
				   
			   }
				
			}
			
			
			if(WCISNIETY_START_KAT){
			
			_delay_ms(80);
			
			   if(WCISNIETY_START_KAT){
				uart_putc('A');

           PORTB ^= (1<<PB1);
				 //  TCNT3 = 0;
	
				   flaga_kat = 1;

			   }
				
			}
				  
			


	}

	return 0;
}

void wlacz_LCD(void){
	
     //ustawiamy na wyjscia PB7, PB6, PB5, PB4, EN(PB3), RS(PB2)
	DDRF = (0xF0) | (1<<EN_LCD) | (1<<RS_LCD);
	PORT_LCD = 0; //Wazne zeby RS_LCD byl na stan niski bo bedziemy wysylac rozkazy
	//czekamy do ustabilizowania sie zasilania (datasheet Wait for more than 15 ms)
    _delay_ms(45);

	//****************INICJALIZACJA WEDLUG INSTRUKCJI (3 razy)********************************

	//1
    PORT_LCD |= (1<<EN_LCD); //sygnalizyjemy wazny transfer danych
    PORT_LCD |= (1<<KONF_5) | (1<<KONF_4);
    PORT_LCD &= ~(1<<EN_LCD);//potwierdzamy opadajacym zboczem EN

	_delay_ms(4.1); //czekamy (datasheet Wait for more than 4.1 ms)

	//2
	PORT_LCD |= (1<<EN_LCD); //sygnalizujemy wazny transfer danych
	PORT_LCD |= (1<<KONF_5) | (1<<KONF_4);
	PORT_LCD &= ~(1<<EN_LCD);//Potwierdzamy opadajacym zboczem EN

	_delay_us(100); //czekamy (datasheet Wait for more than 100 µs)

	//3
	PORT_LCD |= (1<<EN_LCD); //sygnalizujemy wazny transfer danych
	PORT_LCD |= (1<<KONF_5) | (1<<KONF_4);
	PORT_LCD &= ~(1<<EN_LCD); //potwierdzamy opadajacym zboczem EN

	//tutaj juz producent nic nam nie mowi ale zaczekajmy 100 us
	_delay_us(100);

	//*******************KONIEC INICJALIZACJI WEDLUG INSTRUKCJI********************************

	// USTAWIAMY 4 linie transferu danych
	PORT_LCD |= (1<<EN_LCD); //sygnalizujemy wazny transfer danych
	PORT_LCD &= ~(1<<KONF_4);  //zerujemy PB4
	PORT_LCD &= ~(1<<EN_LCD); //potwierdzamy opadajacym zboczem EN

   // USTAWIAMY PARAMETRY WYSWIETLACZA
	//PB3 - 0 jedna linia
	//PB2 - 0 5x8 matryca
	wyslij_do_LCD(0b00100000);

   //USTAWIAMY TRYB PRACY WYSWIETLACZA
	//PB2 - 1 adres po zapisnie danych inkrementacja
	//PB1 - przesun po zapisie kursor
	wyslij_do_LCD(0b00000110);

	//WLACZAMY WYSWIETLACZ
	wyslij_do_LCD(0b00001100);

	PORT_LCD |= (1<<RS_LCD);

     czysc_LCD();

} // koniec wlacz_LCD


void wyslij_do_LCD(uint8_t bajt) {

	//starsza polowka
	PORT_LCD |= (1<<EN_LCD); // sygnalizujemy wazny transfer danych
	PORT_LCD = (bajt & 0xF0) | (PORT_LCD & 0x0F); //cztery najstarsze bity PB7, PB6, PB5, PB4
	PORT_LCD &= ~(1<<EN_LCD); //potwierdzamy opadajacym zboczem EN

     asm volatile("nop"); // czekaj cykl

     //mlodsza polowka
     PORT_LCD |= (1<<EN_LCD); //sygnalizujemy wazny transfer danych
     PORT_LCD = ( (bajt & 0x0F)<<4) | (PORT_LCD & 0x0F); // cztery najmlodsze bity PB3, PB2, PB1, PB0
     PORT_LCD &= ~(1<<EN_LCD); //potwierdzamy opadajacym zboczem EN

     _delay_us(40);
}


void czysc_LCD(void){

	PORT_LCD &= ~(1<<RS_LCD); //zerujemy RS (rozkaz)

	wyslij_do_LCD(1);

	PORT_LCD &= ~(1<<EN_LCD); //potwierdz opadajacym zboczem EN

	PORT_LCD |= (1<<RS_LCD);
	_delay_ms(1.64);
}

void wyswietl_napis(char *napis, uint8_t dlugosc) {

	for(uint8_t i = 0; i < dlugosc; i++){
		wyslij_do_LCD(napis[i]);
	}
}


void linijka(uint8_t gora_dol){
	
	if(gora_dol == 0){
		
		PORT_LCD &= ~(1<<RS_LCD);
		wyslij_do_LCD(0x80);
		PORT_LCD |= (1<<RS_LCD);
	    _delay_us(40);
		
	} else {
		PORT_LCD &= ~(1<<RS_LCD);
		wyslij_do_LCD(0xC0);
		PORT_LCD |= (1<<RS_LCD);
		_delay_us(40);
		
	}
	
}


/*
ISR(TIMER3_COMPA_vect){
	
				   
				PORTB ^= (1<<PB1);
		
	
}*/
/*
ISR(TIMER2_OVF_vect){
	
	PORTB ^= (1<<PB1);
	
}*/