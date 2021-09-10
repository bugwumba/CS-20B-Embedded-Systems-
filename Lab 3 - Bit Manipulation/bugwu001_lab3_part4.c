/*	Partner(s) Name & E-mail: None
 *	Lab Section: 22
 *	Assignment: Lab # 3 Exercise # 4

 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
	Buddy Ugwumba
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void){

    //input 
    DDRA = 0x00; PORTA = 0xFF;
    //outputs
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;

    unsigned char eight_bit = 0x00;
    unsigned char value = 0x00;
    unsigned char lowNib = 0x00;
    unsigned char highNib = 0x00;
    

    while(1){
        eight_bit = PINA;
        value = eight_bit;
        lowNib = (value >> 4);
        PORTB = lowNib;
        value = eight_bit;
        highNib = (value << 4);
        PORTC = highNib;
    }


    return 1;
}