#include "xc.h"
#include "tools.h"
#include <stdio.h>
#include <math.h>


int main(void) {

    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    AD1CON3bits.ADCS = 8;
    AD1CON1bits.ASAM = 1;
    AD1CON1bits.SSRC = 7;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.CHPS = 0;
    
    AD1CON2bits.CSCNA = 1; // scan mode  
    AD1CON2bits.SMPI = 1;
    AD1CSSLbits.CSS5 = 1;
    AD1CSSLbits.CSS11 = 1;
    
    ANSELBbits.ANSB5 = 1;
    ANSELBbits.ANSB11 = 1;
    
    TRISBbits.TRISB11 = 1;
    TRISBbits.TRISB5 = 1;
    
    AD1CON1bits.ADON = 1;
    
    
    
    
    
    init_UART1();
    
    unsigned char m[50];
    int ADCIR,ADCbattery;
    double battery,distance;
    
    AD1CON1bits.SAMP = 1;

    while (1) {
           
        AD1CON1bits.DONE = 0;
        while (!AD1CON1bits.DONE);
        
        ADCIR=ADC1BUF0;
        ADCbattery=ADC1BUF1;

        double x = ADCIR *3.3/1023;     
        distance =  2.34 - 4.74 * x + 4.06 * pow(x, 2) - 1.6 * pow(x, 3) + 0.24 * pow(x, 4);

        double y = ADCbattery *3.3/1023;     

        battery = y*3;
        
        sprintf(m, "Value: %.2f, Battery: %.2f", distance*100, battery); // Format the output

        print_buffer_UART1(m);
        tmr_wait_ms(TIMER1,300);
   }

    return 0;
}




/*
 
 
    AD1CON3bits.ADCS = 8;
    AD1CON1bits.ASAM = 0;
    AD1CON1bits.SSRC = 0;
    AD1CON2bits.CHPS = 0;
 * 
 * 
    AD1CHS0bits.CH0SA = 11;

    ANSELBbits.ANSB11 = 1;
    AD1CON1bits.ADON = 1;
    TRISBbits.TRISB11 = 1;


 
 */