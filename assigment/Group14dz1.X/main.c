#include "xc.h"
#include"tools.h"
#include <stdio.h>
#include <math.h>

//declaring variables
char msg[BUFFER_SIZE];
int counter10ms = 0;
int angle;
int magX = 0, magY = 0, magZ = 0;
CircularBuffer cb;

// an interrupt triggered when the UART is ready to print
void __attribute__((__interrupt__, __auto_psv__))_U1TXInterrupt() {
    //resetting the interrupt flag
    IFS0bits.U1TXIF = 0;
    //printing the queued data
    int data = dequeue(&cb);
    if (data != -1) {
        print_UART1(data);
    }
}

//simulating an algorithm that takes 7ms to execute
void algorithm(){
    tmr_wait_ms(TIMER2, 7);
}

// the main function
int main(void) {
    //clearing the analog registers
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    //initializing the circular buffer that will be queuing the data we want to print
    initCircularBuffer(&cb);
    
    //initializing SPI to communicate with and set up the magnetometer
    init_SPI1();
    setup_mag();
    
    //initiating UART to print results
    init_UART1();
    
    //enabling the TX interrupt, it is triggered when UART is available for printing
    U1STAbits.UTXISEL0 = 0;
    U1STAbits.UTXISEL1 = 0;
    IEC0bits.U1TXIE = 1;
    
    //setting up the period to work at a 100Hz rate
    tmr_setup_period(TIMER1, 10);
    while (1) {
        //simulating an algorithm
        algorithm();

        // the code inside the if condition works at the rate of 25Hz
        if (counter10ms % 4 == 0) {
            // getting the X,Y, and Z data from the magnetometer and calculates the average of every 5 values
            magX = mag_get_x() / 5 + magX;
            magY = mag_get_y() / 5 + magY;
            magZ = mag_get_z() / 5 + magZ;
            
        }
        
        // the code inside the if condition works at the rate of 5Hz
        if (counter10ms == 20) {
            // we calculate the angle to the magnetic north then we prepare the String that needs to be printed
            sprintf(msg, "$MAG,%d,%d,%d*, $YAW,%.3f*", magX, magY, magZ, atan2(magY,magX) * 180.0 / M_PI);
            
            // we queue all the characters of the string to be printed to the UART
            int c = 0;
            //we disable the TX interrupt flag to make sure won't have an interrupt while updating the buffer.
            IEC0bits.U1TXIE = 0;
            do {
                enqueue(&cb, msg[c]);
                c++;
            } while (msg[c ] != '\0');
            
            // separating prints by a space
            enqueue(&cb, ' ');
            
            //we re-enable the interrupt when we are out of critical area
            IEC0bits.U1TXIE = 1;

            // we force the first print out to make sure our interrupts will be triggered correctly
            print_UART1(dequeue(&cb));
            
            // resetting the data to be recollected again
            magX = 0;
            magY = 0;
            magZ = 0;
            
            //we reset the counter to restart
            counter10ms = 0;
        }
        // we ensure the 100Hz rate in case we finish early
        tmr_wait_period(TIMER1);
        
        //a counter to keep everything synchronized
        counter10ms++;
    }
 
    return 0;
}