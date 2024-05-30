#include "xc.h"
#include "tools.h"
#include <stdio.h>
#include <math.h>


parser_state pstate;
//parser_state pstates [MAX_CMD];
int index = -1;
int x,y,z;
int data[4];
CMDCircularBuffer cbCMD;
CircularBuffer cb;

int state=0;

int done = 0;
int counter1ms=0;

// an interrupt triggered when the UART is ready to print
void __attribute__((__interrupt__, __auto_psv__))_U1TXInterrupt() {
    //resetting the interrupt flag
    IFS0bits.U1TXIF = 0;
    //printing the queued data
    int data = dequeue(&cb);
    if(data!=-1){
        U1TXREG = data;
    }

}


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
    
    
    AD1CON3bits.ADCS = 8;
    AD1CON1bits.ASAM = 1;
    AD1CON1bits.SSRC = 7;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.CHPS = 0;
    
    AD1CON2bits.CSCNA = 1; // scan mode  
    AD1CON2bits.SMPI = 1;
    AD1CSSLbits.CSS15 = 1;
    AD1CSSLbits.CSS11 = 1;
    
    ANSELBbits.ANSB15 = 1;
    ANSELBbits.ANSB11 = 1;
    
    TRISBbits.TRISB11 = 1;
    TRISBbits.TRISB15 = 1;
 
    AD1CON1bits.ADON = 1;
    
   
        //enabling the TX interrupt, it is triggered when UART is available for printing
    U1STAbits.UTXISEL0 = 0;
    U1STAbits.UTXISEL1 = 0;
    IEC0bits.U1TXIE = 1;
    
    initCircularBuffer(&cb);

    
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
        
        
    
    
    unsigned char m[BUFFER_SIZE];
    int ADCIR,ADCbattery;
    double battery,distance;
    
    AD1CON1bits.SAMP = 1;
    
//    tmr_setup_period(TIMER2, 1);

    while(1){
        

           
        AD1CON1bits.DONE = 0;
        while (!AD1CON1bits.DONE);
        
        ADCIR=ADC1BUF1;
        ADCbattery=ADC1BUF0;

        double x = ADCIR *3.3/1023;     
        distance =  2.34 - 4.74 * x + 4.06 * pow(x, 2) - 1.6 * pow(x, 3) + 0.24 * pow(x, 4);

        double y = ADCbattery *3.3/1023;     

        battery = y*3;
        
        sprintf(m, "%.2f", distance*100); // Format the output

//
//        // we queue all the characters of the string to be printed to the UART
//        int c = 0;
//        //we disable the TX interrupt flag to make sure won't have an interrupt while updating the buffer.
//        IEC0bits.U1TXIE = 0;
//        do {
//           enqueue(&cb, m[c]);
//           c++;
//        } while (m[c] != '\0');
//
//        // separating prints by a space
//        enqueue(&cb, ' ');
//
//        //we re-enable the interrupt when we are out of critical area
//        IEC0bits.U1TXIE = 1;

//        char data;
        // we force the first print out to make sure our interrupts will be triggered correctly
//        U1TXREG = dequeue(&cb);           


        
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
            int il = 0;
            parser_state temp;
            temp = CMDdequeue(&cbCMD);
            while (nxt < temp.index_payload) {
                data[il] = extract_integer(&temp.msg_payload[nxt]);
                nxt = next_value(&temp.msg_payload, nxt);
//              i = (i +1)%2;
                print_UART1(data[il]);

                il++;

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
