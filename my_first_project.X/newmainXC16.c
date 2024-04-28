#include <xc.h> // Include the appropriate header file for your PIC microcontroller
#include <stdbool.h>
#include"timer.h"
#define TIMER1 1
#define TIMER2 2

/*
void __attribute__((__interrupt__, __auto_psv__))_T1Interrupt() {

    IFS0bits.T1IF = 0;
    LATAbits.LATA0 = !LATAbits.LATA0;


}
 */

/*
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
 */
int main(void) {



    /*
    TRISAbits.TRISA0 = 0;
    
   tmr_setup_period(TIMER1, 200);
   while (1) {
       tmr_wait_period(TIMER1);
       LATAbits.LATA0 = !LATAbits.LATA0;
   } 
  
    
     */



    /*
        TRISAbits.TRISA0 = 0;
        LATAbits.LATA0 = 1;
        while (1) {

            LATAbits.LATA0 = !LATAbits.LATA0;
            tmr_wait_ms(TIMER1, 100);

        }
     */

    // timer exo3
    /*

        // initialization code
        int ret;
        TRISAbits.TRISA0 = 0;
        TRISGbits.TRISG9 = 0;
        tmr_setup_period(TIMER1, 200);
        int delay = 1000; // 200, 2000
        while (1) {
            // code to blink LD1
            tmr_wait_ms(TIMER2, delay);

            LATAbits.LATA0 = !LATAbits.LATA0;
            ret = tmr_wait_period(TIMER1);
            if (ret) {

                // turn on LD2
                LATGbits.LATG9 = 1;
            } else {
                // turn off LD2
                LATGbits.LATG9 = 0;
            }
        }




     */



    // test timer
    /*
        while (1) {
            TRISAbits.TRISA0 = 0;
            LATAbits.LATA0 = !LATAbits.LATA0;
            int ms = 200;
            unsigned long long ticks = (ms/ 1000.0)*281250; // Using unsigned long long to support larger values

            // Configure the timer and its period register accordingly

            T1CONbits.TON = 0; // Disable Timer1 during configuration
            //T1CONbits.TCS = 0; // Select internal clock (Fosc/4)
            T1CONbits.TCKPS = 0b11; // Set prescaler to 1:256
            TMR1 = 0; // Clear Timer1 value

            // Adjust the period register and handle overflow
            while (ticks > 0xFFFF) {
                ticks -= 0xFFFF;
                PR1 = 0xFFFF; // Set maximum value for Timer1 period
                T1CONbits.TON = 1; // Enable Timer1
                // Wait for Timer1 period to complete
                tmr_wait_period(TIMER1);
            }
            T1CONbits.TON = 0;
            PR1 = (unsigned int) ticks; // Set remaining ticks as period register
            T1CONbits.TON = 1; // Enable Timer1
            tmr_wait_period(TIMER1);
            //T1CONbits.TON = 0;


        }

     */



    // interrupt exo2 
    /*
        ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
        TRISAbits.TRISA0 = 0;
        while (1) {
            TRISEbits.TRISE8 = 1;
            RPINR0bits.INT1R = 88;
            INTCON2bits.GIE = 1;
            IFS1bits.INT1IF = 0;
            IEC1bits.INT1IE = 1;

        }
     */


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

    // remap input pin U1RXR to RD11
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







}
