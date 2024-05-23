#include "xc.h"
#include "tools.h"
#include <stdio.h>
#include <math.h>
CircularBuffer cb;


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
    
    
    
        init_UART1();

        //enabling the TX interrupt, it is triggered when UART is available for printing
    U1STAbits.UTXISEL0 = 0;
    U1STAbits.UTXISEL1 = 0;
    IEC0bits.U1TXIE = 1;
    
    
    
        initCircularBuffer(&cb);

    
    unsigned char m[BUFFER_SIZE];
    int ADCIR,ADCbattery;
    double battery,distance;
    
    AD1CON1bits.SAMP = 1;
    
    tmr_setup_period(TIMER1, 1);
    int i = 0;
    while (1) {
           
        AD1CON1bits.DONE = 0;
        while (!AD1CON1bits.DONE);
        
        ADCIR=ADC1BUF1;
        ADCbattery=ADC1BUF0;

        double x = ADCIR *3.3/1023;     
        distance =  2.34 - 4.74 * x + 4.06 * pow(x, 2) - 1.6 * pow(x, 3) + 0.24 * pow(x, 4);

        double y = ADCbattery *3.3/1023;     

        battery = y*3;
        
        if(i==100){
            
            sprintf(m, "$SENS,%.2f,%.2f*", distance*100, battery); // Format the output
            
            
             // we queue all the characters of the string to be printed to the UART
            int c = 0;
            //we disable the TX interrupt flag to make sure won't have an interrupt while updating the buffer.
            IEC0bits.U1TXIE = 0;
            do {
                enqueue(&cb, m[c]);
                c++;
            } while (m[c] != '\0');
            
            // separating prints by a space
            enqueue(&cb, ' ');
            
            //we re-enable the interrupt when we are out of critical area
            IEC0bits.U1TXIE = 1;
            
            char data;
            // we force the first print out to make sure our interrupts will be triggered correctly
            U1TXREG = dequeue(&cb);           
            

            i = 0;       
        }

        tmr_wait_period(TIMER1);
        i++;
   }

    return 0;
}

