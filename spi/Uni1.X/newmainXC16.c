/*
 * File:   newmainXC16.c
 * Author: YNS
 *
 * Created on April 28, 2024, 6:38 PM
 */

#define CS TRISDbits.TRISD6

#include "xc.h"
#include "newxc16_header.h"

int main(void) {
    // ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    int read_addr, value_from_chip, trash;
    
    //configuring the SPI
    SPI1CON1bits.MSTEN = 1; // master mode
    SPI1CON1bits.MODE16 = 0; // 8?bit mode
    SPI1CON1bits.PPRE = 3; // 1:1 primary prescaler
    SPI1CON1bits.SPRE = 3; // 5:1 secondary prescaler
    SPI1STATbits.SPIEN = 1; // enable SPI

    //setting up the CS and making sure theyre not selected
    TRISAbits.TRISA1 = 1; // RA1?RPI17 MISO
    TRISFbits.TRISF12 = 0; //RF12?RP108 SCK
    TRISFbits.TRISF13 = 0; // RF13?RP109 MOSI

    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISDbits.TRISD6 = 0;

    LATBbits.LATB3 = 1;
    LATBbits.LATB4 = 1;
    LATDbits.LATD6 = 1;

    //reamapping the pins
    RPINR20bits.SDI1R = 0b0010001; // MISO (SDI1) ? RPI17
    RPOR12bits.RP109R = 0b000101; // MOSI (SDO1) ? RF13
    RPOR11bits.RP108R = 0b000110; // SCK1;

    // remap input pin U1RXR to RD11
    TRISDbits.TRISD11 = 1;
    RPINR18bits.U1RXR = 75;
    // remap output pin U1TX to RD0
    TRISDbits.TRISD0 = 0;
    RPOR0bits.RP64R = 1;

    U1BRG = 468; // 72M / (16 * 9600) - 1
    U1MODEbits.UARTEN = 1; // enable UART
    U1STAbits.UTXEN = 1;

    CS = 0; // change to the PORT connected to the chip select

    // we try to read whats on 0x40 to get the  ID
    read_addr = 0x40;

    // we even tried rereading the value we already modified we got
    //read_addr = 0x4B;

    // we send the address where we want to write (0x4B)
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = 0x4B;

    while (SPI1STATbits.SPIRBF == 0);
    trash = SPI1BUF;

    // we send 0x01 to be written on 0x4B to to go the sleep mode
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = 0x01;
    while (SPI1STATbits.SPIRBF == 0);
    trash = SPI1BUF;

    // we wait for 2ms
    tmr_wait_ms(TIMER1, 2);

    // we send the address where we want to write (0x4C)
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = 0x4C;

    while (SPI1STATbits.SPIRBF == 0);
    trash = SPI1BUF;


    // we send 0x00 to be written on 0x4C to go to Normal mode
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = 0x00;
    while (SPI1STATbits.SPIRBF == 0);
    trash = SPI1BUF;

    // we send the address where we want to read (0x40; and later trying 0x4B)
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = read_addr | 0x80; // setting the MSB to 1
    while (SPI1STATbits.SPIRBF == 0);
    trash = SPI1BUF; // read to prevent buffer overrun

    // we read from the specified address
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = 0x00; // clocking out zeros so that the other chip can send the
    while (SPI1STATbits.SPIRBF == 0);
    value_from_chip = SPI1BUF; // get the value from the register

    // printing the received value
    U1TXREG = value_from_chip;


    CS = 1;

    while (1);


    return 0;
}