#include "xc.h"
#include "tools.h"
#include <stdio.h>
#include <math.h>


parser_state pstate;
int index = -1;
int x,y,z;
int data[4];
CMDCircularBuffer cbCMD;
CircularBuffer cb;

int state=0;

int counter1ms=0;

// an interrupt triggered when the UART is ready to print
void __attribute__((__interrupt__, __auto_psv__))_U1TXInterrupt() {
    //resetting the interrupt flag
    IFS0bits.U1TXIF = 0;
    //printing the queued data
    int printable = dequeue(&cb);
    if(printable!=-1){
        U1TXREG = printable;
    }

}


// an interrupt triggered when the UART is ready to print
void __attribute__((__interrupt__, __auto_psv__))_U1RXInterrupt() {
   
    //resetting the interrupt flag
    IFS0bits.U1RXIF = 0;
    char reced = U1RXREG;
    
    if(parse_byte(&pstate,reced)){
        if(CMDisFull(&cbCMD)){
            print_buffer_UART1("$MACK,0*");

        }else{
            CMDenqueue(&cbCMD,pstate);
            print_buffer_UART1("$MACK,1*");
            index = (index +1) % MAX_CMD;
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
    init_UART1(1,1);
        
    // initializing parser state
	pstate.state = STATE_DOLLAR;
	pstate.index_type = 0; 
	pstate.index_payload = 0;
    
    TRISEbits.TRISE8 = 1;
    RPINR0bits.INT1R = 88;

    INTCON2bits.GIE = 1;
    IFS1bits.INT1IF = 0;
    IEC1bits.INT1IE = 1;
   
    TRISAbits.TRISA0 = 0;
    LATAbits.LATA0 = 0;
    
    TRISBbits.TRISB9 = 0;
    LATBbits.LATB9 = 1;

    init_LED();
    
//    LIGHTLEFT = 1;
//    LIGHTRIGHT = 1;
//    LIGHTBREAKS = 1;
//    LIGHTLOW = 1;
//    LIGHTBEAM = 1;
    
    double battery,distance;
    tmr_setup_period(TIMER2, 1);
    while(1){
        
        get_distance_and_battery(&distance, &battery);
        
        unsigned char m[BUFFER_SIZE];

        sprintf(m, "%.2f ", distance*100); // Format the output
        if (counter1ms % 60 == 0) {
            enqueue_buffer(m, &cb);
        }
           
        switch(state){
            case STATE_WAIT:
                break;
            case STATE_EXECUTE:                
                if (!CMDisEmpty(&cbCMD)) {       
                  int nxt = 0;
                  parser_state temp;
                  temp = CMDdequeue(&cbCMD);
                  for (int i = 0; nxt < temp.index_payload;i++) {
                      data[i] = extract_integer(&temp.msg_payload[nxt]);
                      nxt = next_value(&temp.msg_payload, nxt);
                      enqueue(&cb,data[i]);
                  }
                }  
            break;
            
            case STATE_EXECUTING:
                break;

        }
          
        counter1ms++;
        tmr_wait_period(TIMER2);
    }
    
    return 0;
}