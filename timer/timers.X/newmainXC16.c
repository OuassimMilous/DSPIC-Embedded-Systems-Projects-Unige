#include <xc.h> // Include the appropriate header file for your PIC microcontroller
#include <stdbool.h>
#include "timer.h"

int main(void) {
    TRISAbits.TRISA0 = 0;
      
    while(1){
    
        LATAbits.LATA0 = !LATAbits.LATA0;
        tmr_wait_ms(TIMER2,200);
        LATAbits.LATA0 = !LATAbits.LATA0;
        tmr_wait_ms(TIMER2,20);                 
    }
    return 0;
}