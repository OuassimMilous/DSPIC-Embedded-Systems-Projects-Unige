
/*
 * File:   newmainXC161.c
 * Author: YNS
 *
 * Created on April 18, 2024, 4:45 PM
 */


#include <xc.h> // Include the appropriate header file for your PIC microcontroller
#include <stdbool.h>
#include"timer.h"
#define TIMER1 1
#define TIMER2 2
int i = 0;

void __attribute__((__interrupt__, __auto_psv__))_T1Interrupt() {

    IFS0bits.T1IF = 0;
    LATGbits.LATG9 = !LATGbits.LATG9;


}

void __attribute__((__interrupt__, __auto_psv__))_U1RXInterrupt() {

    IFS0bits.U1RXIF = 0;
    i++;


}

void __attribute__((__interrupt__, __auto_psv__))_INT1Interrupt() {
    IFS1bits.INT1IF = 0;
    IEC0bits.T2IE = 1;
    tmr_setup_period(TIMER2, 1);
}

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt() {
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 0;
    T2CONbits.TON = 0;
    IEC1bits.INT1IE = 1;
    if (PORTEbits.RE9 == 1) {
        // toggle led 
        LATAbits.LATA0 = !LATAbits.LATA0;
    }

    void algorithm() {
        tmr_wait_ms(TIMER2, 7);
    }

    int main(void) {

        TRISGbits.TRISG9 = 0;
        TRISAbits.TRISA0 = 0;
        int ret;
        // remap input pin U1RXR to RD11
        TRISDbits.TRISD11 = 1;
        RPINR18bits.U1RXR = 75;
        // remap output pin U1TX to RD0
        TRISDbits.TRISD0 = 0;
        RPOR0bits.RP64R = 1;
        U1BRG = 468; // 72M / (16 * 9600) - 1
        U1MODEbits.UARTEN = 1; // enable UART
        U1STAbits.UTXEN = 1;
        U1STAbits.URXISEL = 0b01;
        IEC0bits.U1RXIE = 1;
        int c;

        
             
         

        tmr_setup_period(TIMER1, 10);
        while (1) {
            algorithm();
            // code to handle the assignment




            c = U1RXREG;
            if (c == 1) {

                LATAbits.LATA0 = !LATAbits.LATA0;

            }
            U1TXREG = i;

            ret = tmr_wait_period(TIMER1);
        }
        
        return 0;
    }


