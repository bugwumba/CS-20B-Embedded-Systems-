#include <avr/interrupt.h>

unsigned char receivedData = 0x00;

// Master Code
void SPI_MasterInit(void) {
    // Set DDRB to have MOSI, SCK, and SS as output and MISO as input
    DDRB = 0xB0;
    // Set SPCR register to enable SPI, enable master, 
    // and use SCK frequency of fosc/16 (pg. 168)
    SPCR |= 0x51;
    SREG |= 0x80;
}

void SPI_MasterTransmit(char cData) {
    // data in SPDR will be transmitted, e.g. SPRD = cData;
    SPDR = cData;
    // set SS low
    PORTB = 0xA0;
    while(!(SPSR & (1<<SPIF))) { // Wait for transmission complete */ 
        ;         
    }
    // set SS high
    PORTB = 0xB0;

}

//Servant code
void SPI_SlaveInit(void) {
    // set DDRB to have MISO line as output and MOSI, SCK and SS as input
    DDRB = 0x40;
    // set SPCR register to enable SPI and enable SPI interrupt (pg. 168)
    SPCR |= 0xC0;
    // make sure global interrupts are enabled on SREG register (pg. 9)
    SREG |= 0x80;
}

ISR(SPI_STC_vect) { // this is enabled in the SPCR register's "SPI 
                    // Interrupt Enable"
        // SPDR  contains the received data, e.g. unsigned char receivedData =
        //SPDR;
        receivedData = SPDR;
}
