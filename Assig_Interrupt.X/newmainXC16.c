#include <xc.h> // Include the appropriate header file for your PIC microcontroller
#include <stdbool.h>
#include"timer.h"
#define TIMER1 1
#define TIMER2 2

void __attribute__((__interrupt__, __auto_psv__))_T1Interrupt() {

    IFS0bits.T1IF = 0;
    LATAbits.LATA0 = !LATAbits.LATA0;


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

}

int main(void) {


    //Interrupt exo1
    /*
        TRISAbits.TRISA0 = 0;
   


        while (1) {
            IEC0bits.T1IE = 1;
        
            tmr_setup_period(TIMER1, 50);
             while(!IFS0bits.INT0IF){
            }



        }
     */

    // interrupt exo2 

    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    TRISAbits.TRISA0 = 0;
    while (1) {
        TRISEbits.TRISE8 = 1;
        RPINR0bits.INT1R = 88;
        INTCON2bits.GIE = 1;
        IFS1bits.INT1IF = 0;
        IEC1bits.INT1IE = 1;

    }

    return 0;
}
