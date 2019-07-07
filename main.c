/*
 * main.c
 *
 *  Created on: 30 lis 2016
 *      Author: fet
 */

#include"serwa/serwa.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "analogi/analog.h"
// piny sterujace tranzystorami wlaczajacymi cewki silnika krokowego
#define t1 (1<<PD3)
#define t2 (1<<PD4)
#define t3 (1<<PD5)
#define t4 (1<<PD6)
// definicje poszczegulnych krokow silnika unipolarnego
#define krok1 PORTD |= t1 ; PORTD &= ~(t2|t3|t4)
#define krok2 PORTD |= t2 ; PORTD &= ~(t1|t3|t4)
#define krok3 PORTD |= t3 ; PORTD &= ~(t1|t2|t4)
#define krok4 PORTD |= t4 ; PORTD &= ~(t1|t2|t3)
#define STOP PORTD &= ~ (t1|t2|t3|t4)
// definicja pinow obslugujacych klawisze odpowiedzialne za wlaczenie obrotu
#define klawisz1 (1<<PB6)
#define klawisz2 (1<<PB7)
// definicje graniczne analogow
#define GRANICA_DOL 490
#define GRANICA_GORA 536
#define PRZYSPIESZENIE_RUCHU 20
// deklaracje zmiennych i funkcji
u08 t;
volatile u16 wynik,licznik1;
u16 analog_dol( u16 pwm ,u16 wynik);
u16 analog_gora(u16 pwm , u16 wynik);
void kroki_prawo(void);
void kroki_lewo(void);
// funkcja gluwna
int main (void){
	DDRD |= t1|t2|t3|t4;				// piny portu D jako wyjscia
	PORTD &= ~ (t1|t2|t3|t4);			// ustawienie na liniach sterujacych stan niski
	DDRB &= ~(klawisz1|klawisz2);		// piny obslugujace klawiszw jako wejscia
	PORTB |= (klawisz1|klawisz2);		// ustawienie na nich stanu wysokiego
	serwa_int();
	analog_int();
	sei();
	zezwolenie_pomiaru = 1;
	t=1;
	pwm1 = 75;
	pwm2 = 75;
	pwm3 = 100;
	pwm4 = 100;
while(1){

/* pierwszy analog */
	if((zezwolenie_pomiaru == 1) && (t==1)){	// warunki ustalajace kolejnosc i czas pomiaru
		licznik1++;
		if(licznik1>600)licznik1=0;				// licznik odmierzajacy 0,1 ms
		wynik=pomiar_analogu(0);
		if(wynik < GRANICA_DOL)
		{
			pwm1 = analog_dol(pwm1,wynik);


		 }
		else if(wynik>GRANICA_GORA)
		{
			pwm1 = analog_gora(pwm1,wynik);

		}
		zezwolenie_pomiaru = 0;
		t=2;}
/* 2 drugi analog*/

	if((zezwolenie_pomiaru == 1) && (t==2)){
			wynik=pomiar_analogu(1);
			if(wynik < GRANICA_DOL)
			{
				pwm2 = analog_dol(pwm2,wynik);

			}
			else if(wynik>GRANICA_GORA){
				pwm2 = analog_gora(pwm2,wynik);
			}
			zezwolenie_pomiaru = 0;
			t=3;}

/*3 trzeci analog*/
	if((zezwolenie_pomiaru == 1) && (t==3))
			{
				wynik=pomiar_analogu(2);
				if(wynik < GRANICA_DOL)				// wynik to pomiar z analogu
				 {
				  pwm3 = analog_dol(pwm3,wynik);

				 }
				  else if(wynik>GRANICA_GORA)
					{
					 pwm3 = analog_gora(pwm3,wynik);
					}
				zezwolenie_pomiaru = 0;
				t=4;
			}

/*4 czwarty analog*/
	if((zezwolenie_pomiaru == 1) && (t==4))
		{
				wynik=pomiar_analogu(3);
				if(wynik < GRANICA_DOL)
				{
				   pwm4 = analog_dol(pwm4,wynik);

				}
				else if(wynik>GRANICA_GORA){
				 pwm4 = analog_gora(pwm4,wynik);
				}
				zezwolenie_pomiaru = 0;
				t=5;
		}
/*5 pi¹ty analog*/
		if((zezwolenie_pomiaru == 1) && (t==5)){
			wynik = pomiar_analogu(4);
			if(wynik <GRANICA_DOL){
				pwm5 = analog_dol(pwm5,wynik);
			}
			else if(wynik<GRANICA_GORA){
				pwm5 = analog_gora(pwm5,wynik);
			}
			zezwolenie_pomiaru = 0;
			t=1;

		}
		if(zezwolenie_na_krok ==1){
			if(!(klawisz1 & PINB)){			//klawisze w stanie niskim ,wlaczenie nastepuje po do³aczeniu wysokiego
				kroki_prawo();
			}
			else if(!(klawisz2 & PINB)){
				kroki_lewo();
			}
			else STOP;
			zezwolenie_na_krok = 0;

		}

}
}
void kroki_prawo(void){
	static u08 kr;
	if(kr == 0){krok1;}
	if(kr == 1){krok2;}
	if(kr == 2){krok3;}
	if(kr == 3){krok4;}
	if(++kr > 3) kr=0;


}
void kroki_lewo(void){
	static u08 kr;
	if(kr == 0){krok4;}
	if(kr == 1){krok3;}
	if(kr == 2){krok2;}
	if(kr == 3){krok1;}
	if(++kr > 3)kr = 0;
}
u16 analog_dol(u16 pwm ,u16 wynik){
	   wynik=(wynik+100)/100;
	   wynik=wynik*PRZYSPIESZENIE_RUCHU;
	   if(!(licznik1%wynik))pwm--;	//reszta z dzielenia przez wynik(od1-6)
	   if(pwm<46)pwm=45;
	   return pwm;
}

u16 analog_gora(u16 pwm ,u16 wynik){
	         wynik=(1128-wynik)/100;
			 wynik=wynik*PRZYSPIESZENIE_RUCHU;
			 if(!(licznik1%wynik))pwm++;
			 if(pwm>104)pwm=105;
			 return pwm;

}
