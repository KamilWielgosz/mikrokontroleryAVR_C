#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define RS_LCD 2
#define EN_LCD 3
#define PORT_LCD PORTF
#define KONF_5 PF5
#define KONF_4 PF4

void wlacz_LCD(void);
void czysc_LCD(void);
void wyslij_do_LCD(uint8_t bajt);
void wyswietl_napis(char *napis, uint8_t dlugosc);

int main(){
	wlacz_LCD();

	char napis[10] = "TESTOWANIE";

	wyswietl_napis(napis, 10);


	float zmien = 3.14;

	PORT_LCD &= ~(1<<RS_LCD);
		wyslij_do_LCD(0xC0);
	PORT_LCD |= (1<<RS_LCD);
			_delay_us(40);

			char bufor[4] = "    ";
			dtostrf(zmien, 0, 4, bufor);

	wyswietl_napis(bufor, 4);


	while(1);

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

	_delay_ms(4.1);; //czekamy (datasheet Wait for more than 4.1 ms)

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

	PORT_LCD |= (1<<EN_LCD);  //sygnalizujemy wazny transfer danych
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
