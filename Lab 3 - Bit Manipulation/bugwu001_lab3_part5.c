/*	Partner(s) Name & E-mail: None
 *	Lab Section: 22
 *	Assignment: Lab # 3 Exercise # 5

 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
	Buddy Ugwumba
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main (void) {

    //only PB0 is an input;
    //But still must set the PORT to be an output
    DDRB = 0xFE; PORTB = 0x00;
    //all 8 pins are inputs
    DDRD = 0x00; PORTD = 0xFF;

    unsigned short weight_D = 0x00;
    unsigned short weight_B = 0x00;
    unsigned int weight = 0x00;
    unsigned char airbag = 0x00;

    while(1){
        weight_D = PIND;
        weight_B = PINB;
        airbag = 0x00;
        weight = ((weight_D << 1) + (weight_B & 0x01));
        if(weight >= 0x46){
            airbag = 0x02;
        }
        if((weight > 0x05) && (weight < 0x46)){
            airbag = 0x04;
        }
         PORTB = airbag;
    }

    return 1;
}