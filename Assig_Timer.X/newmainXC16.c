/*
 * File:   newmainXC16.c
 * Author: YNS
 *
 * Created on April 18, 2024, 9:53 PM
 */

#include <xc.h> // Include the appropriate header file for your PIC microcontroller
#include <stdbool.h>
#include"timer.h"
#define TIMER1 1
#define TIMER2 2

int main(void) {


    // exo1 
    
    /*
 
     TRISAbits.TRISA0 = 0;

        tmr_setup_period(TIMER1, 200);
        while (1) {
            tmr_wait_period(TIMER1);
            LATAbits.LATA0 = !LATAbits.LATA0;
        }
 
 
 
 
     */





    //exo3
    
    int ret;
    TRISAbits.TRISA0 = 0;
    TRISGbits.TRISG9 = 0;
    tmr_setup_period(TIMER1, 200);
    int delay = 100; // 200, 2000
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



    return 0;
}
