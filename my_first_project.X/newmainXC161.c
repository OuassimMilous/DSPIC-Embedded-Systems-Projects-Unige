
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

void __attribute__((__interrupt__, __auto_psv__))_T1Interrupt() {

    IFS0bits.T1IF = 0;
    LATGbits.LATG9 = !LATGbits.LATG9;


}
int main(void) {
    
      TRISGbits.TRISG9 = 0;
   


        while (1) {
            IEC0bits.T1IE = 1;
        
            tmr_setup_period(TIMER1, 50);
             while(!IFS0bits.INT0IF){
            }



        }
    
    
    
    return 0;
}
