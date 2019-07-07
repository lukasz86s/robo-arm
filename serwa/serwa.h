/*
 * serwa.h
 *
 *  Created on: 30 lis 2016
 *      Author: fet
 */

#ifndef STEROWANIE_RAMIENIA_SERWA_SERWA_SERWA_H_
#define STEROWANIE_RAMIENIA_SERWA_SERWA_SERWA_H_

#include <avr/io.h>

#define serwo1 (1<<PB1)
#define serwo2 (1<<PB2)
#define serwo3 (1<<PB3)
#define serwo4 (1<<PB4)
#define serwo5 (1<<PB5)


typedef uint8_t u08;
typedef uint16_t u16;

volatile u16 pwm1,pwm2,pwm3,pwm4,pwm5;
volatile u08 zezwolenie_pomiaru,zezwolenie_na_krok;
void serwa_int (void);

#endif /* STEROWANIE_RAMIENIA_SERWA_SERWA_SERWA_H_ */
