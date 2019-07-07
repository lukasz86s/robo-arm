/*
 * analog.c
 *
 *  Created on: 9 gru 2016
 *      Author: fet
 */
#include "analog.h"
#include <avr/io.h>



void analog_int (void){
	DDRC &= ~(kontroler_1|kontroler_2|kontroler_3|kontroler_4|kontroler_5);				// ustawienie pinow jako wejscia
	PORTC |= kontroler_1|kontroler_2|kontroler_3|kontroler_4|kontroler_5;				// podciagniecie pinow pod vcc
	// ustawienia ADC
	ADCSRA|= (1<<ADPS1)|(1<< ADPS2)|(1<<ADEN); 	// w³aczenie i ustawiene preskalera na 64
	ADMUX |= (1<<REFS0);						// odniesienie do VCC
}

u16 pomiar_analogu(u08 kanal){

		ADMUX = (ADMUX & 0xf8)| kanal;							// ustawienie wybranego kana³u ewentulanie = kanal|0x01000000
		ADCSRA |=(1<<ADSC);					   //start pomiaru
		while(ADCSRA & (1<<ADSC));
		return ADCW ;

}
