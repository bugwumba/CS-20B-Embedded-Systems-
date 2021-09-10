/*	Partner(s) Name & E-mail: None
 *	Lab Section: 22
 *	Assignment: Lab # 4 Exercise # 4
 *	Exercise Description: [optional - include for your own benefit] 

A household has a digiital combination deadbolt lock system on the dorrway. 
The system has buttons on the keypad. Button "X" connects to PA0, "Y" connects to PA1,


 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
	Buddy Ugwumba
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum STATES {Start, Init, Wait, Lock, Unlock} State;
unsigned char tmpB = 0x00;

void CombLock_Tick(){

    switch (State){ //Transitions
        case Start:
            State = Init; break;
        case Init:
            if(tmp == 0x04){ State = Wait; } 
            else{ State = Init; }
            break;
        case Wait:
            if(tmp == 0x00){ State = Wait; }
            else if(tmp == 0x02){ State = Unlock; }
            else { State = Lock; }
            break;
        case Unlock:
            if(tmp == 0x04){ 
                if(tmp == 0x00){
                    if(tmp == 0x02){
                        State = Lock;
                    }
                }
                State = Wait;
            }
            else if(tmp == 0x80){ State = Lock; } 
            else { State = Unlock; }
            break;
        case Lock:
            if(tmp == 0x04){ State = Wait; }
            if(tmp == 0x80) { State = Unlock; }
            else { State = Lock; }
            break;
        default:
            printf("%s", "ERROR: Reached default state transition.\n");
            State = Init;
            break;

    }

    switch(State) {//State Actions
        case Unlock:
            tmpB = 0x01;
            break;
        default:
            printf("%s", "ERROR: Reached default state action.\n");
            tmpB = 0x00;
            break;
    }

}

int main (void){

    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    State = Start;
    PORTB = tmpB;

    while(1){
        CombLock_Tick();
        PORTB = tmpB;
    }
    return 1;
}