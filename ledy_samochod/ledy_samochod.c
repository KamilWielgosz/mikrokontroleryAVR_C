#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED1 (1<<PB1)
#define LED2 (1<<PB2)
#define PRZYCISK1 (1<<PC5)
#define PRZYCISK2 (1<<PC4)
#define PRZYCISK3 (1<<PC3)
#define PRZYCISK4 (1<<PC2)
#define WCISNIETY1 !(PINC & PRZYCISK1)
#define WCISNIETY2 !(PINC & PRZYCISK2)
#define WCISNIETY3 !(PINC & PRZYCISK3)
#define WCISNIETY4 !(PINC & PRZYCISK4)

uint8_t pier_drug = 0;

int main(void)
{
   DDRB |= LED1 | LED2;   //Wyjœcia LED
   PORTC |= PRZYCISK1 | PRZYCISK2 | PRZYCISK3 | PRZYCISK4;

   //Inicjalizacja Timer1 (PWM)
   TCCR1A |= (1<<COM1A1) //Zmiana stanu wyjœcia OC1A na niski przy porównaniu A
          |  (1<<COM1B1) //Zmiana stanu wyjœcia OC1B na niski przy porównaniu B
          |  (1<<WGM11); //Tryb 14 (FAST PWM, TOP=ICR1)

   TCCR1B |= (1<<WGM13) | (1<<WGM12)  //Tryb 14 (FAST PWM, TOP=ICR1)
          | (1<<CS10);                //Brak preskalera

   ICR1 = 1000;  //Wartoœæ maksymalna (dla trybu 14)
                 //a wiêc czêstotliwoœæ = CLK/ICR1 = 1kHz

   OCR1A=500;    //Wartoœæ pocz¹tkowa porównania A (Wyjœcie OC1A - PB1),
                 //wype³nenie = 50%

   OCR1B=500;    //Wartoœæ pocz¹tkowa porównania B (Wyjœcie OC1B - PBB),
                 //wype³nenie = 50%


   sei();    //Globalne uruchomienie przerwañ

   while(1){//Pêtla nieskoñczona


  if(WCISNIETY4) {

	  if(pier_drug == 0){
		  pier_drug = 1;
	  } else {
		  pier_drug = 0;
	  }

  }

    if(WCISNIETY1){

    	_delay_ms(80);

    	if(WCISNIETY1){

    	  if(pier_drug == 0){
    		if(OCR1A < 1000){

    			OCR1A += 10;

    		}
    	  } else {
    		  if(OCR1B < 1000){

    			  OCR1B += 10;

    		  }
    	  }

    	}


    }

    if(WCISNIETY2){

     	_delay_ms(80);

     	if(WCISNIETY2){

     	  if(pier_drug == 0){
     		if(OCR1A > 0){

     			OCR1A -= 10;

     		}
     	  } else {

     		  if(OCR1B > 0){

     			  OCR1B -= 10;

     		  }

     	  }


     	}


     }

    if(WCISNIETY3){

     	_delay_ms(80);

     	if(WCISNIETY3){

     		 TCCR1A ^= (1<<COM1A1) |  (1<<COM1B1);
        _delay_ms(600);
     	}


     }



   }
}


