#include "xc.h"
#include "tools.h"

parser_state pstate;
int x,y,z;
int data[4];
//CircularBufferCMD cbParser;
int done = 0;
// an interrupt triggered when the UART is ready to print
void __attribute__((__interrupt__, __auto_psv__))_U1RXInterrupt() {
    //resetting the interrupt flag
    IFS0bits.U1RXIF = 0;
    char reced = U1RXREG;
    if(parse_byte(&pstate,reced)){
      done = 1;  
    }
}


int main(void) {
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
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


    while(1){
        
          if (done) {
            int nxt = 0;
            int i = 0;
            while (nxt < pstate.index_payload) {
                data[i] = extract_integer(&pstate.msg_payload[nxt]);
                nxt = next_value(pstate.msg_payload, nxt);
                i++;
            }

            print_UART1(data[0]);
            print_UART1(data[1]);
            print_UART1(data[2]);
            done = 0;
        }
          
         
    }
    
    return 0;
}
