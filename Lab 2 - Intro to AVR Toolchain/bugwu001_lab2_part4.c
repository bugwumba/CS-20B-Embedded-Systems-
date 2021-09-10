/*	Partner(s) Name & E-mail: None
 *	Lab Section: 22
 *	Assignment: Lab # 2 Exercise # 4
 *	Exercise Description: [optional - include for your own benefit] A garage door sensor connect to PA0 (1 means door is open), and a light sensor connects to PA1 (1 means light is sensed). Write a program that illuminates an LED connected to PB0 (1 means illuminate) if the garage door is open at night.
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
	Buddy Ugwumba
 */

#include <stdio.h>
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <math.h>

int main (void){

    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0x00; PORTC = 0xFF;
    DDRD = 0xFF; PORTD = 0x00;

    unsigned int total_weight = 0x00;
    unsigned int PassA_Weight = 0x00;
    unsigned int PassB_Weight = 0x00;
    unsigned int PassC_Weight = 0x00;
    unsigned int temp_holder = 0x00;

    while(1){
        PassA_Weight = ~PINA;
        PassB_Weight = ~PINB;
        PassC_Weight = ~PINC;
        total_weight = (PassA_Weight + PassB_Weight + PassC_Weight);
        if(total_weight >= 0x8D){
            temp_holder = (temp_holder | 0x01);
        }
        if(fabs(PassA_Weight - PassC_Weight) > 0x50){
            temp_holder = (temp_holder | 0x02);
        }

        total_weight = (total_weight >> 2);
        total_weight = (total_weight & 0xFC);

        PORTD = (total_weight | temp_holder) - 0x01;
    }

    return 0;

}
