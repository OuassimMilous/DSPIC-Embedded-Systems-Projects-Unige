#include "xc.h"
#include "tools.h"

parser_state pstate;
//parser_state pstates [MAX_CMD];
int index = -1;
int x,y,z;
int data[4];
CMDCircularBuffer cbCMD;

int state=0;

int done = 0;
// an interrupt triggered when the UART is ready to print
void __attribute__((__interrupt__, __auto_psv__))_U1RXInterrupt() {
    //resetting the interrupt flag
    IFS0bits.U1RXIF = 0;
    char reced = U1RXREG;
    

    if(parse_byte(&pstate,reced)){
//      done = 1; 
        if(CMDisFull(&cbCMD)){
            print_buffer_UART1(" $MACK,0*");

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
    T2CONbits.TON = 0;   
    
    
    if (PORTEbits.RE8 == 1) {
        // toggle led 
        state = !state;
        print_UART1(state);
    }


}

void __attribute__((__interrupt__, __auto_psv__))_INT1Interrupt() {
    IFS1bits.INT1IF = 0;
    IEC0bits.T1IE = 1;
    tmr_setup_period(TIMER1, 50);
}


int main(void) {
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    CMDinitCircularBuffer(&cbCMD);
    //initiating UART to print results
    init_UART1();
    
    //enabling the RX interrupt, it is triggered when UART is available for printing
    U1STAbits.URXISEL0 = 0;
    U1STAbits.URXISEL1 = 0;
    IEC0bits.U1RXIE = 1;
    
    // initializing parser state
	pstate.state = STATE_DOLLAR;
	pstate.index_type = 0; 
	pstate.index_payload = 0;


         
        TRISEbits.TRISE8 = 1;
        RPINR0bits.INT1R = 88;

        INTCON2bits.GIE = 1;
        IFS1bits.INT1IF = 0;
        IEC1bits.INT1IE = 1;
        
    while(1){
        

      
        
        
        switch(state){
            case 0:
                
                break;
            case 1:
                
                
          if (!CMDisEmpty(&cbCMD)) {
              
//            print_buffer_UART1(pstates[0].msg_payload);
//            print_buffer_UART1(pstates[0].msg_type);

//            print_buffer_UART1(pstates[index-1].msg_payload);
//            print_buffer_UART1(pstates[index-1].msg_type);

//            print_UART1(index);
            
            int nxt = 0;
            int i = 0;
            parser_state temp;
            temp = CMDdequeue(&cbCMD);
            while (nxt < temp.index_payload) {
                data[i] = extract_integer(&temp.msg_payload[nxt]);
                nxt = next_value(&temp.msg_payload, nxt);
//              i = (i +1)%2;
                print_UART1(data[i]);

                i++;

            }

//            print_UART1(data[0]);
//            print_UART1(data[1]);
//            print_UART1(data[2]);
            done = 0;
            
                break;
                
        }
        
        }
          
 
    }
    
    return 0;
}
