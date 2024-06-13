#include "xc.h"
#include "tools.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include<stdbool.h>

parser_state pstate;
int data[2];

CMDCircularBuffer cbCMD;
CircularBuffer cb;

int state = STATE_WAIT;

int counter1ms = 0;
int counterMotor = 0;
char printable;
bool x=0;


// an interrupt triggered when the UART is ready to print

void __attribute__((__interrupt__, __auto_psv__))_U1TXInterrupt() {
    //resetting the interrupt flag
    IFS0bits.U1TXIF = 0;
    //printing the queued data
    char printable;
    if (dequeue(&cb, &printable)) {
        U1TXREG = printable;
    }

}


// an interrupt triggered when the UART is ready to print

void __attribute__((__interrupt__, __auto_psv__))_U1RXInterrupt() {

    //resetting the interrupt flag
    IFS0bits.U1RXIF = 0;
    char reced = U1RXREG;

    if (parse_byte(&pstate, reced)) {
        if (CMDisFull(&cbCMD)) {
            //print_buffer_UART1("$MACK,0*");
            enqueue_buffer(&cb,"$MACK,0*");

        } else {
            if (strcmp(pstate.msg_type, "PCCMD") == 0) {
                CMDenqueue(&cbCMD, pstate);
                //print_buffer_UART1("$MACK,1*");
                enqueue_buffer(&cb,"$MACK,1*");
            }

        }

    }
}

void __attribute__((__interrupt__, __auto_psv__))_T1Interrupt() {

    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 0;
    T1CONbits.TON = 0;

    if (PORTEbits.RE8 == 1) {

        state = !state;
    }


}

void __attribute__((__interrupt__, __auto_psv__))_INT1Interrupt() {
    IFS1bits.INT1IF = 0;
    IEC0bits.T1IE = 1;
    tmr_setup_period(TIMER1, 50);
}

int main(void) {
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    init_adc();

    initCircularBuffer(&cb);

    CMDinitCircularBuffer(&cbCMD);

    //initiating UART to print results and enable RX and TX interrupts
    init_UART1(1, 1);

    // initializing parser state
    pstate.state = STATE_DOLLAR;
    pstate.index_type = 0;
    pstate.index_payload = 0;

    TRISEbits.TRISE8 = 1;
    RPINR0bits.INT1R = 88;

    INTCON2bits.GIE = 1;
    IFS1bits.INT1IF = 0;
    IEC1bits.INT1IE = 1;


    TRISGbits.TRISG9 = 0;
    LATGbits.LATG9 = 0;

    set_up_PWM_wheels();

    //    TRISAbits.TRISA0 = 0;
    //    LATAbits.LATA0 = 0;

    IRENABLE = 1;

    init_LED();

    //    LIGHTLEFT = 1;
    //    LIGHTRIGHT = 1;
    //    LIGHTBREAKS = 1;
    //    LIGHTLOW = 1;
    //    LIGHTBEAM = 1;
    //    LED = 1;

    double battery, distance;
    tmr_setup_period(TIMER2, 1);

    while (1) {

        get_distance_and_battery(&distance, &battery);

        unsigned char m[BUFFER_SIZE];
        //        if (counter1ms % 1000 == 0) {
        //
        //            sprintf(m, "$MBATT,%.2f*$MDIST,%.2f*", battery, distance); // Format the output
        //            enqueue_buffer(&cb, m);
        //            counter1ms = 0; 
        //        
        //        }else if (counter1ms % 100 == 0){
        //            sprintf(m, "$MDIST,%.2f*", distance); // Format the output
        //            enqueue_buffer(&cb, m);          
        //           
        //
        //        }

        if (counter1ms % 100 == 0) {
            sprintf(m, "$MDIST,%.2f*", distance); // Format the output
            enqueue_buffer(&cb, m);
            x=1;

            


        }

        if (counter1ms % 1000 == 0) {
            sprintf(m, "$MBATT,%.2f*", battery); // Format the output
            enqueue_buffer(&cb, m);




        }
        if(!isEmpty(&cb) && x){
//            if (dequeue(&cb, &printable)) {
//                U1TXREG = printable;
//            }
                IFS0bits.U1TXIF = 1;

            x=0;
            
        
        
        
        }





        switch (state) {
            case STATE_WAIT:
                stop_moving();
                if (counter1ms % 500 == 0) {
                    LED = !LED;
                    LIGHTLEFT = !LIGHTLEFT;
                    LIGHTRIGHT = !LIGHTRIGHT;
                }
                break;
            case STATE_EXECUTE:
                //                turnoff_lights();
                LIGHTLEFT = 0;
                LIGHTRIGHT = 0;
                if (counter1ms % 500 == 0) {
                    LED = !LED;
                }

                if (!CMDisEmpty(&cbCMD)) {
                    int nxt = 0;
                    parser_state temp;
                    CMDdequeue(&cbCMD, &temp);
                    for (int i = 0; nxt < temp.index_payload; i++) {
                        data[i] = extract_integer(&temp.msg_payload[nxt]);
                        nxt = next_value(&temp.msg_payload, nxt);

                        enqueue(&cb, data[i]);
                    }

                    state = STATE_EXECUTING;
                    counterMotor = 0;


                }
                break;
            case STATE_EXECUTING:

                if (distance >= 20) {
                    move(data[0], PWMFREQUENCY * 50);
                } else {
                    stop_moving();
                }

                if (counterMotor % (data[1]) == 0) {
                    state = STATE_EXECUTE;
                    stop_moving();
                }


                break;

        }

        counter1ms++;
        counterMotor++;
        LATGbits.LATG9 = tmr_wait_period(TIMER2);
    }

    return 0;
}