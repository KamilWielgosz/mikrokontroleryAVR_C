#define F_CPU 1000000UL
#include<avr/io.h>
#include<util/delay.h>


void Inicjacja_spi()
{
    DDRB = ( 1 << PB5 ) | ( 1 << PB3 ) | ( 1 << PB2 );              //MOSI, SCK, SS' jako wyjœcia
    SPCR = ( 1 << SPE ) | ( 1 << MSTR ) | ( 1 << SPR1 ) | ( 1 << SPR0 );   //W³¹czamy SPI,
}                                                                               //uk³ad Master, najmniejsza czêstotliwoœæ SCK fosc / 128


void Wyslij_spi(int8_t bajt)
{
    SPDR = bajt;                    //Wysy³amy zawartoœæ zmiennej bajt
    while( ! bit_is_set( SPSR, SPIF ) );        //Oczekujemy na zakoñczenie transmisji ( do ustawienia SPIF ) przez sprzêt
}

int main()
{
    int8_t bajt = 0;
    Inicjacja_spi();

    while(1)                //Pêtla nieskoñczona
    {

        Wyslij_spi(bajt);
        _delay_ms(1000);        //Czekamy ok. 1 sek

        bajt++;                //Zwiêkszamy wartoœæ w bajt o 1
        if(bajt == 80){
        	bajt = 1;
        }

    }
    return 0;
}
