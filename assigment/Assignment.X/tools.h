
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> 

#define TIMER1 1
#define TIMER2 2
#define CS LATDbits.LATD6

// Function prototypes
void tmr_wait_ms(int timer, int ms);
void init_UART1();
void print_UART1(unsigned char msg);
void init_SPI1();
void write_SPI1(unsigned char addr, unsigned char msg);
unsigned char read_SPI1(unsigned char addr);


#endif	/* XC_HEADER_TEMPLATE_H */

