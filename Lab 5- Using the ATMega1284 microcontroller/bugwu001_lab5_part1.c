/*	Partner(s) Name & E-mail: None
 *	Lab Section: 22
 *	Assignment: Lab # 5 Exercise # 1

 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
	Buddy Ugwumba
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main (void) {
    //inputs
    DDRA = 0xF0; PORTA = 0xFF;
    //outputs
    DDRC = 0xFF; PORTC = 0x00;

    unsigned char fuel_Lvl_Sensor = 0x00;
    unsigned char ledLight = 0x00;

    //Forever Loop
    while(1){

	//why did inverting PINA work?
        fuel_Lvl_Sensor = ~PINA & 0x0F;
        // 1 means light, 0 means don't light
        if(fuel_Lvl_Sensor == 0x00){
            ledLight = 0x40;
        }
        if((fuel_Lvl_Sensor >= 0x01) && (fuel_Lvl_Sensor <= 0x02)){
            //PC5 lights
            //Also light PC6 because the fule level is less than 4
            ledLight = 0x60;
        }
        if((fuel_Lvl_Sensor >= 0x03) && (fuel_Lvl_Sensor <= 0x04)){
            //PC5 & PC4
            //Also light PC6 if the fuel level is less than 4
                ledLight = 0x70;
        }
        if((fuel_Lvl_Sensor >= 0x05) && (fuel_Lvl_Sensor <= 0x06)){
            //PC5 & PC3
            ledLight = 0x38;
        }
        if((fuel_Lvl_Sensor >= 0x07) && (fuel_Lvl_Sensor <= 0x09)){
            //PC5 & PC2
            ledLight = 0x3C;
        }
        if((fuel_Lvl_Sensor >= 0x0A) && (fuel_Lvl_Sensor <= 0x0C)){
            //PC5 & PC1
            ledLight = 0x3E;
        }
        if((fuel_Lvl_Sensor >= 0x0D) && (fuel_Lvl_Sensor <= 0x0F)){
            //PC5 & PC0
            ledLight = 0x3F;
        }

        PORTC = ledLight;

    }

    return 1;
}