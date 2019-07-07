/*
 * analog.h
 *
 *  Created on: 9 gru 2016
 *      Author: fet
 */

#ifndef ANALOGI_ANALOG_H_
#define ANALOGI_ANALOG_H
#include <avr/io.h>
#define kontroler_1 (1<<PC0)
#define kontroler_2 (1<<PC1)
#define kontroler_3 (1<<PC2)
#define kontroler_4 (1<<PC3)
#define kontroler_5 (1<<PC4)

typedef uint8_t u08;
typedef uint16_t u16 ;

u16 pomiar_analogu(u08 kanal);

// funkcje
void analog_int (void);




#endif /* ANALOGI_ANALOG_H_ */
