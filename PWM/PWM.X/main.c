#include "xc.h"
#include "tools.h"
#include <stdio.h>
#include <stdbool.h>


bool state = 0;

void __attribute__((__interrupt__, __auto_psv__))_T1Interrupt() {

    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 0;
    T2CONbits.TON = 0;   
    
    
    if (PORTEbits.RE9 == 1) {
        // toggle led 
        state = !state;
    }


}

void __attribute__((__interrupt__, __auto_psv__))_INT1Interrupt() {
    IFS1bits.INT1IF = 0;
    IEC0bits.T1IE = 1;
    tmr_setup_period(TIMER1, 1);
}



int main(void) {

//  //setting PWM
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    set_up_PWM_wheels();
    
        TRISEbits.TRISE8 = 1;
        RPINR0bits.INT1R = 88;

        INTCON2bits.GIE = 1;
        IFS1bits.INT1IF = 0;
        IEC1bits.INT1IE = 1;
        
        
    while(1){
        if(state){
            move(FORWARD,PWMFREQUENCY * 50);
        }else{
            stop_moving();

        }
//       
        
//        move(FORWARD,PWMFREQUENCY * 50);
//        tmr_wait_ms(TIMER1,3000);
//        
//        move(RIGHT,PWMFREQUENCY * 50);
//        tmr_wait_ms(TIMER1,3000);
//        
//        move(BACKWARDS,PWMFREQUENCY * 50);
//        tmr_wait_ms(TIMER1,3000);
//
//        
//        move(LEFT,PWMFREQUENCY * 50);
//        tmr_wait_ms(TIMER1,3000);

    }
    
    
    return 0;
}
