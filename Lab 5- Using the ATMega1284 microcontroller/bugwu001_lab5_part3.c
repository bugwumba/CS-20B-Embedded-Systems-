/* Author: 
 * Partner(s) Name (if applicable):   
 * Lab Section: 
 * Assignment: Lab #5  Exercise #3 
 * Exercise Description: [optional - include for your own benefit] 
 * Counts from 0 to 63 and then downwardcs
 * 
 * I acknowledge all content contained herein, excluding template or example 
 * code, is my own original work. 
 * Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 * https://drive.google.com/file/d/1uXVWvZkYpiMRTAanp005e2Cuf-FZ-rU5/view?usp=sharing
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <stdio.h>

enum STATES {WAIT, INCREMENT, DECREMENT, RESET, CLEAR} State;
unsigned char value = 0x00;
unsigned char lowValue = 0x3F;
unsigned char activateReverse = 0x00;
unsigned char temp;

//MUST COUNT EACH BUTTON PRESS ONCE
void ButtonCalc_Tick(){

    switch(State){//Transitions
   
        case WAIT:
            if(temp == 0x00) { State = WAIT; }
            if(temp == 0x01) { 
                if(value == 0x3F) { 
                    State = RESET; 
                }
                else if(activateReverse == 0x01){
                    State = DECREMENT;
                }
                else{
                    State = INCREMENT; 
                }
            }
            break;

        case INCREMENT:
            if(temp == 0x00){ State = WAIT; }
            if(temp == 0x01){ State = CLEAR; }
            break;

        case DECREMENT:
            if(temp == 0x00){ State = WAIT; }          
            if(temp == 0x01){ State = CLEAR; }
            break;

        case RESET:
            if(temp == 0x00) { State = WAIT; }
            if(temp == 0x01) { State = CLEAR; }
            break;

        case CLEAR:
            if(temp == 0x00){ State = WAIT; }
            if(temp == 0x01){ State = CLEAR; }
            break;


        default:
            printf("%s", "Error: Reached default state transition.\n"); State = WAIT; break; 
    }

    switch(State){//State actions
        case WAIT:
            break;
        case INCREMENT:
            if(value < 0x3F){ value++;} break;
        case DECREMENT:
            if(lowValue > 0){ 
                lowValue--;
            }
            else{
                activateReverse = 0x00;
                lowValue = 0x3F;
            } 
            break;
        case RESET:
            value = 0x00;
            activateReverse = 0x01; 
            break;
        case CLEAR:
            break;
        default:
            printf("%s", "Error: Reached default state action.\n"); break;    
    }
}

int main (void){

    //input is zero
    //output is 1
    DDRA = 0xFE; PORTA = 0xFF;
    DDRB = 0x3F; PORTB = 0x00;

    State = WAIT;
    PORTB = value;

    while (1){
        temp = ~PINA & 0xFF;
        ButtonCalc_Tick();
        if(activateReverse != 0x01){
            PORTB = value;
        }
        else{
            PORTB = lowValue;
        }
    }

    return 1;
}