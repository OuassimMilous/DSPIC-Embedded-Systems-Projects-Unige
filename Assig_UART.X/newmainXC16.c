#include <xc.h> // Include the appropriate header file for your PIC microcontroller
#include <stdbool.h>
#include"timer.h"
#define TIMER1 1
#define TIMER2 2
int i = 0;
unsigned long long j = 0;
int c;
int x;
int counter=0;
bool s=0;

void __attribute__((__interrupt__, __auto_psv__))_U1RXInterrupt() {

    IFS0bits.U1RXIF = 0;
    i++;



}

/*
 
 void __attribute__((__interrupt__, __auto_psv__))_T1Interrupt() {

    IFS0bits.T1IF = 0;
    LATGbits.LATG9 = !LATGbits.LATG9;


}
 
 */


void __attribute__((__interrupt__, __auto_psv__))_INT1Interrupt() {
    IFS1bits.INT1IF = 0;
    IEC0bits.T1IE = 1;
    // tmr_setup_period(TIMER1, 1);
    // U1TXREG = i;
    x = 1;

}

void __attribute__((__interrupt__, __auto_psv__))_INT2Interrupt() {
    IFS1bits.INT2IF = 0;
    IEC0bits.T1IE = 1;
    // tmr_setup_period(TIMER1, 1);
    //U1TXREG = j;
    x = 2;

}

void __attribute__((__interrupt__, __auto_psv__))_T1Interrupt() {
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 0;
    //T2CONbits.TON = 0;
    IEC1bits.INT1IE = 1;
    if (x == 1) {
        // toggle led 
        // LATAbits.LATA0 = !LATAbits.LATA0;
        U1TXREG = i;
        x = 0;
    }
    if (x == 2) {
        // toggle led 
        // LATAbits.LATA0 = !LATAbits.LATA0;
        U1TXREG = j;
        x = 0;
    }
}

void algorithm() {
    tmr_wait_ms(TIMER2, 7);
}

int main(void) {
    /*
     TRISDbits.TRISD11 = 1;
        RPINR18bits.U1RXR = 75;
        // remap output pin U1TX to RD0
        TRISDbits.TRISD0 = 0;
        RPOR0bits.RP64R = 1;



        U1BRG = 468; // (7372800 / 4) / (16 * 9600) = 1
        U1MODEbits.UARTEN = 1; // enable UART
        U1STAbits.UTXEN = 1; // enable U1TX (must be after UARTEN)

        while (1) {
            U1TXREG = 'c';
        }
     */
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
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
    U1STAbits.URXISEL = 0;
    IEC0bits.U1RXIE = 1;

    TRISEbits.TRISE8 = 1;
    RPINR0bits.INT1R = 88;
    TRISEbits.TRISE9 = 1;
    RPINR1bits.INT2R = 89;
    INTCON2bits.GIE = 1;
    IFS1bits.INT1IF = 0;
    IEC1bits.INT1IE = 1;
    IEC1bits.INT2IE = 1;

    tmr_setup_period(TIMER1, 10);
    while (1) {
        algorithm();
        // code to handle the assignment




        c = U1RXREG;
        if (c == 1) {

            LATAbits.LATA0 = !LATAbits.LATA0;

        }
        if (c == 2) {

            s=!s;

        }
        
        


        ret = tmr_wait_period(TIMER1);
        if (ret == 1) {
            j++;
            ret = 0;

        }
        
        if (s == 1) {
            counter++;
            if (counter == 20) {
                counter = 0;
                LATGbits.LATG9 = !LATGbits.LATG9;
            }

        }
        if (s == 0) {
            LATGbits.LATG9=0;

        }

    }

    return 0;
}





