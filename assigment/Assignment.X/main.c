#include "xc.h"
#include "tools.h"

int main(void){
    TRISAbits.TRISA0 = 0;

    while(1){
        LATAbits.LATA0 = 1;
        tmr_wait_ms(TIMER1,2000);
        LATAbits.LATA0 = 0;
        tmr_wait_ms(TIMER1,1000);
    }
    return 0;
}
