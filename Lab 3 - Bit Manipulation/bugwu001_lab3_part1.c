/*	Partner(s) Name & E-mail: None
 *	Lab Section: 22
 *	Assignment: Lab # 3 Exercise # 1
 *	Exercise Description: [optional - include for your own benefit] 
 *	Write a C  program that counts the number of 1's on ports A
 *  and B and outputs that number of on portC
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
	Buddy Ugwumba
 */

#include <avr/io.h> 
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main (void) {

    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;

    unsigned char amount = 0x00;

    while(1){
        for(unsigned char i = 0; i < 8; i++){
            if((PINA >> i) & 0x01){
                amount++;
            }
            if((PINB >> i) & 0x01){
                amount++;
            }
        }
        PORTC = amount;
        amount = 0;
    }
    return 1;
}

