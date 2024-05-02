
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> 

#define TIMER1 1
#define TIMER2 2
#define CS LATDbits.LATD6

#define MAGCHIPID 0x40
#define MAGSLEEPMODE 0x4B
#define MAGOPMODE 0x4C

#define MAGXLSB 0x42
#define MAGXMSB 0x43

#define MAGYLSB 0x44
#define MAGYMSB 0x45

#define MAGZLSB 0x46
#define MAGZMSB 0x47

#define MAGHALLLSB 0x48
#define MAGHALLMSB 0x49



// Function prototypes
void tmr_setup_period(int timer, int ms);
int tmr_wait_period(int timer);
void tmr_wait_ms(int timer, int ms);
void init_UART1();
void print_UART1(unsigned char msg);
void print_buffer_UART1(char buffer[]);

void init_SPI1();
void write_SPI1(unsigned char addr, unsigned char msg);
unsigned char read_SPI1(unsigned char addr);
void setup_mag();
int mag_get_x();
int mag_get_y();
int mag_get_z();

#endif	/* XC_HEADER_TEMPLATE_H */

