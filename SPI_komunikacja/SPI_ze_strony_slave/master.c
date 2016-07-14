#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define RS_LCD 2
#define EN_LCD 3

void Inicjacja_spi()
{
    DDRB = ( 1 << PB4 );        //MISO jako wyjœcie
    SPCR = ( 1 << SPE );            //W³¹czamy SPI
}

int8_t Odbieraj_spi()
{                            //Czekamy na koniec transmisji danych
    while ( ! bit_is_set( SPSR, SPIF ) );        // ( a¿ do ustawienie flagi SPIF    )
    return SPDR;                    //Zwracamy to co dostaliœmy do SPDR
}                            //Czyli to co wys³a³ Master



void wlacz_LCD(void);
void czysc_LCD(void);
void wyslij_do_LCD(uint8_t bajt);
void wyswietl_napis(char *napis, uint8_t dlugosc);

int main(){
	wlacz_LCD();
	Inicjacja_spi();

	char napis[8] = "QWERTYUI";

	wyswietl_napis(napis, 8);


			char czysc[8] = "        ";
			char bufor[8] = "        ";

	int8_t bajt = 0;
	while(1){
			   bajt = Odbieraj_spi();

			   PORTD &= ~(1<<RS_LCD);
			   	wyslij_do_LCD(0xC0);
			   PORTD |= (1<<RS_LCD);

			   		_delay_us(40);
			   		wyswietl_napis(czysc, 8);

			   	PORTD &= ~(1<<RS_LCD);
			   			wyslij_do_LCD(0xC0);
			   	PORTD |= (1<<RS_LCD);

			   		_delay_us(40);

			   		dtostrf(bajt, 0, 8, bufor);

			   		wyswietl_napis(bufor, 8);
	}

	return 0;
}

void wlacz_LCD(void){
     //ustawiamy na wyjscia PB7, PB6, PB5, PB4, EN(PB3), RS(PB2)
	DDRD = (0xF0) | (1<<EN_LCD) | (1<<RS_LCD);
    PORTD = 0; //Wazne zeby RS_LCD byl na stan niski bo bedziemy wysylac rozkazy
	//czekamy do ustabilizowania sie zasilania (datasheet Wait for more than 15 ms)
    _delay_ms(45);

	//****************INICJALIZACJA WEDLUG INSTRUKCJI (3 razy)********************************

	//1
	PORTD |= (1<<EN_LCD); //sygnalizyjemy wazny transfer danych
	PORTD |= (1<<PD5) | (1<<PD4);
	PORTD &= ~(1<<EN_LCD);//potwierdzamy opadajacym zboczem EN

	_delay_ms(4.1);; //czekamy (datasheet Wait for more than 4.1 ms)

	//2
	PORTD |= (1<<EN_LCD); //sygnalizujemy wazny transfer danych
	PORTD |= (1<<PD5) | (1<<PD4);
	PORTD &= ~(1<<EN_LCD);//Potwierdzamy opadajacym zboczem EN

	_delay_us(100); //czekamy (datasheet Wait for more than 100 µs)

	//3
	PORTD |= (1<<EN_LCD); //sygnalizujemy wazny transfer danych
	PORTD |= (1<<PD5) | (1<<PD4);
	PORTD &= ~(1<<EN_LCD); //potwierdzamy opadajacym zboczem EN

	//tutaj juz producent nic nam nie mowi ale zaczekajmy 100 us
	_delay_us(100);

	//*******************KONIEC INICJALIZACJI WEDLUG INSTRUKCJI********************************

	// USTAWIAMY 4 linie transferu danych
	PORTD |= (1<<EN_LCD); //sygnalizujemy wazny transfer danych
	PORTD &= ~(1<<PD4);  //zerujemy PB4
	PORTD &= ~(1<<EN_LCD); //potwierdzamy opadajacym zboczem EN

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

    PORTD |= (1<<RS_LCD);

     czysc_LCD();

} // koniec wlacz_LCD


void wyslij_do_LCD(uint8_t bajt) {

	//starsza polowka
	 PORTD |= (1<<EN_LCD); // sygnalizujemy wazny transfer danych
	 PORTD = (bajt & 0xF0) | (PORTD & 0x0F); //cztery najstarsze bity PB7, PB6, PB5, PB4
	 PORTD &= ~(1<<EN_LCD); //potwierdzamy opadajacym zboczem EN

     asm volatile("nop"); // czekaj cykl

     //mlodsza polowka
     PORTD |= (1<<EN_LCD); //sygnalizujemy wazny transfer danych
     PORTD = ( (bajt & 0x0F)<<4) | (PORTD & 0x0F); // cztery najmlodsze bity PB3, PB2, PB1, PB0
     PORTD &= ~(1<<EN_LCD); //potwierdzamy opadajacym zboczem EN

     _delay_us(40);
}


void czysc_LCD(void){

	PORTD |= (1<<EN_LCD);  //sygnalizujemy wazny transfer danych
	PORTD &= ~(1<<RS_LCD); //zerujemy RS (rozkaz)

	wyslij_do_LCD(1);

	PORTD &= ~(1<<EN_LCD); //potwierdz opadajacym zboczem EN

    PORTD |= (1<<RS_LCD);
	_delay_ms(1.64);
}

void wyswietl_napis(char *napis, uint8_t dlugosc) {

	for(uint8_t i = 0; i < dlugosc; i++){
		wyslij_do_LCD(napis[i]);
	}
}
