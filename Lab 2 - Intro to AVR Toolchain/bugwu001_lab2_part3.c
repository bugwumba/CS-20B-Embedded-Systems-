/*	Partner(s) Name & E-mail: None
 *	Lab Section: 22
 *	Assignment: Lab # 2 Exercise # 3
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

int main (void) {
    DDRA = 0x00; PORTA = 0xff;
    DDRC = 0xFF; PORTC = 0x00;

    unsigned char cntavail = 0x00;
    unsigned char parking_spot = 0x00;
    unsigned char cars_parked = 0x00;
    
    while (1) {
        parking_spot = PINA & 0x0F;
        if(parking_spot == 0x08 || parking_spot == 0x04 || parking_spot == 0x02 || parking_spot == 0x01){
            cars_parked = 0x01;
            //SetBit(PORTC, 7, 0);
        }
        else if(parking_spot == 0x0C || parking_spot == 0x05 || parking_spot == 0x03 || parking_spot == 0x06 || parking_spot == 0x09 || parking_spot == 0x0A){
            cars_parked = 0x02;
            //SetBit(PORTC, 7, 0);
        }
        else if(parking_spot == 0x0E || parking_spot == 0x07 || parking_spot == 0x0B || parking_spot == 0x0D){
            cars_parked = 0x03;
            //SetBit(PORTC, 7, 0);
        }
        else if (parking_spot ==  0x0F){
            cars_parked = 0x04;
            //SetBit(PORTC, 7, 1);
        }
        else{
            cars_parked = 0x00;
            //SetBit(PORTC, 7, 0);
        }
        cntavail = (0x04 - cars_parked);
        PORTC = cntavail;
        if (cars_parked == 0x04){
                PORTC =  0x80;
        }
    }

    return 1;
}