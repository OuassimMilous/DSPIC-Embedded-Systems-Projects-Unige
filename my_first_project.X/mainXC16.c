#include <xc.h> // Include the appropriate header file for your PIC microcontroller
int main(void) {
    // Set RA0 as an output pin
    TRISAbits.TRISA0 = 0; // Clear the bit to configure RA0 as an output
    TRISEbits.TRISE8=1;
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
   /* while(1){
        
        if(PORTEbits.RE8==1){
           LATAbits.LATA0=1;
        }
        else{
          LATAbits.LATA0=0;  
                
        }
    }
    */
    while(1){
        LATAbits.LATA0=PORTEbits.RE8;
    
    
    }
    // Your code here
    
    return 0;
}