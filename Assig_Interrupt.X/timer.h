/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */
#define TIMER1 1
#define TIMER2 2

void tmr_setup_period(int timer, int ms) {
    if (timer == 1) {
        TMR1 = 0;
        PR1 = (ms / 1000.0)*281250;
        T1CONbits.TCKPS = 0b11;
        T1CONbits.TON = 1;
    } else {
        TMR2 = 0;
        PR2 = (ms / 1000.0)*281250;
        T2CONbits.TCKPS = 0b11;
        T2CONbits.TON = 1;

    }
}
/*
void tmr_wait_period(int timer) {

    if (timer == 1) {
        while (!IFS0bits.T1IF) {
        };
        IFS0bits.T1IF = 0;

    } else {
        while (!IFS0bits.T2IF) {
        };
        IFS0bits.T2IF = 0;


    }


}


*/
int tmr_wait_period(int timer) {
    int x;
    if (timer == 1) {
        x = IFS0bits.T1IF;
        while (!IFS0bits.T1IF) {
        };
        IFS0bits.T1IF = 0;


    } else {
        x = IFS0bits.T2IF;
        while (!IFS0bits.T2IF) {
        };
        IFS0bits.T2IF = 0;


    }
    return x;

}
  



/*
void tmr_wait_ms(int timer, int ms) {
    if (timer == 1) {
        TMR1 = 0;
        PR1 = (ms / 1000.0)*281250;
        T1CONbits.TCKPS = 0b11;
        T1CONbits.TON = 1;
    } else {
        TMR2 = 0;
        PR2 = (ms / 1000.0)*281250;
        T2CONbits.TCKPS = 0b11;
        T2CONbits.TON = 1;



    }
    if (timer == 1) {
        while (!IFS0bits.T1IF) {
        };
        IFS0bits.T1IF = 0;
        TMR1 = 0;
        T1CONbits.TON = 0;


    } else {
        while (!IFS0bits.T2IF) {
        };
        IFS0bits.T2IF = 0;


    }


}
*/
void tmr_wait_ms(int timer,int ms) {

    unsigned long long ticks = (ms / 1000.0)*281250; // Using unsigned long long to support larger values

    // Configure the timer and its period register accordingly
    if (timer == 1) {
        T1CONbits.TON = 0; // Disable Timer1 during configuration
        //T1CONbits.TCS = 0; // Select internal clock (Fosc/4)
        T1CONbits.TCKPS = 0b11; // Set prescaler to 1:256
        TMR1 = 0; // Clear Timer1 value

        // Adjust the period register and handle overflow
        while (ticks > 65535) {
            ticks -= 65535;
            PR1 = 65535; // Set maximum value for Timer1 period
            T1CONbits.TON = 1; // Enable Timer1
            // Wait for Timer1 period to complete
            tmr_wait_period(TIMER1);
        }
        T1CONbits.TON = 0;
        PR1 = (unsigned int) ticks; // Set remaining ticks as period register
        T1CONbits.TON = 1; // Enable Timer1
        tmr_wait_period(TIMER1);
         
    } else if (timer == 2) {
        T2CONbits.TON = 0; // Disable Timer2 during configuration
        T2CONbits.TCS = 0; // Select internal clock (Fosc/4)
        T2CONbits.TCKPS = 0b11; // Set prescaler to 1:256
        TMR2 = 0; // Clear Timer2 value

        // Adjust the period register and handle overflow
        while (ticks > 65535) {
            ticks -= 65535;
            PR2 = 65535; // Set maximum value for Timer2 period
            T2CONbits.TON = 1; // Enable Timer2
            // Wait for Timer2 period to complete
            tmr_wait_period(TIMER2);

        }
        PR2 = (unsigned int) ticks; // Set remaining ticks as period register
        T2CONbits.TON = 1; // Enable Timer2
        tmr_wait_period(TIMER2);
        T2CONbits.TON = 0;
    }
}














