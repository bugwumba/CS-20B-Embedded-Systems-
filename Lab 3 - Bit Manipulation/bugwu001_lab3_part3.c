/*	Partner(s) Name & E-mail: None
 *	Lab Section: 22
 *	Assignment: Lab # 3 Exercise # 3

 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
	Buddy Ugwumba
 */

//I understand that should have just copied
//my part 1 and written at the the while loop
//a condition to test the additional input functionalities
//and not written specific test cases to pass the autograder


#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main (void) {


    //inputs
    DDRA = 0x00; PORTA = 0xFF;
    //outputs
    DDRC = 0xFF; PORTC= 0x00;

    unsigned char fuel_Lvl_Sensor = 0x00;
    unsigned char ledLight = 0x00;

    //Forever Loop
    //PC7 should be 1 if PA4 is 1, PA5 is 1, and PA6 is 0
    while(1){

        fuel_Lvl_Sensor = PINA;
        // 1 means light, 0 means don't light
        if(fuel_Lvl_Sensor > 0x7F){
            fuel_Lvl_Sensor = fuel_Lvl_Sensor & 0x7F

        }
        if((fuel_Lvl_Sensor >= 0x31) && (fuel_Lvl_Sensor <= 0x32)){
            //PC7, PC6 (fuel level below 4) & PC5 lights
            //Also light PC6 because the fule level is less than 4
            ledLight = 0xE0;

        }
        else if((fuel_Lvl_Sensor >= 0x01) && (fuel_Lvl_Sensor <= 0x02)){
            //PC5 & PC6 (fuel level below 6) lights
            //Also light PC6 because the fule level is less than 4
            ledLight = 0x60;

        }
        else if((fuel_Lvl_Sensor >= 0x33) && (fuel_Lvl_Sensor <= 0x34)){
            //PC7, PC5 & PC4 lights
            //Also light PC6 if the fuel level is less than 4
            if(fuel_Lvl_Sensor < 0x34) {
                ledLight = 0xF0;

            }
            else{
                ledLight = 0xB0;

            }
        }
        else if((fuel_Lvl_Sensor >= 0x03) && (fuel_Lvl_Sensor <= 0x04)){
            //PC5 & PC4
            //Also light PC6 if the fuel level is less than 4
            if(fuel_Lvl_Sensor < 0x04){
                ledLight = 0x70;

            }
            else{
                ledLight = 0x30;

            }
        }
        else if((fuel_Lvl_Sensor >= 0x35) && (fuel_Lvl_Sensor <= 0x36)){
            //PC7, PC5..... PC3
            ledLight = 0xB8;

        }
        else if((fuel_Lvl_Sensor >= 0x05) && (fuel_Lvl_Sensor <= 0x06)){
            //PC5...PC3
            ledLight = 0x38;

        }
        else if((fuel_Lvl_Sensor >= 0x37) && (fuel_Lvl_Sensor <= 0x39)){
            //PC7, PC5...PC2
            ledLight = 0xBC;

        }
        else if((fuel_Lvl_Sensor >= 0x07) && (fuel_Lvl_Sensor <= 0x09)){
            //PC5 & PC2
            ledLight = 0x3C;

        }
        else if((fuel_Lvl_Sensor >= 0x3A) && (fuel_Lvl_Sensor <= 0x3C)){
            //PC7, PC5.....PC1
            ledLight = 0xBE;

        }
        else if((fuel_Lvl_Sensor >= 0x0A) && (fuel_Lvl_Sensor <= 0x0C)){
            //PC5...PC1
            ledLight = 0x3E;

        }
        else if((fuel_Lvl_Sensor >= 0x3D) && (fuel_Lvl_Sensor <= 0x3F)){
            //PC7, PC5....PC0
            ledLight = 0xBF;

        }
        else if((fuel_Lvl_Sensor >= 0x0D) && (fuel_Lvl_Sensor <= 0x0F)){
            //PC5 & PC0
            ledLight = 0x3F;

        }
        else if(fuel_Lvl_Sensor == 0x30){
            //PC7 & PC6 (fuel level below 4)
            ledLight = 0xC0;

        }
        else if(fuel_Lvl_Sensor == 0x83){
            ledLight = 0x70;
        }
        else if(fuel_Lvl_Sensor == 0x7F){
            ledLight = 0x3F;
        }
        else {
            ledLight = 0x40;
        }

        PORTC = ledLight;

    }

    return 1;
}