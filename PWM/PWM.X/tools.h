#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> 

//defining constants
#define TIMER1 1
#define TIMER2 2
#define CS LATDbits.LATD6
#define BUFFER_SIZE 37

//defining registers

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

#define RIGHTB OC1R 
#define RIGHTF OC2R 

#define LEFTB OC3R 
#define LEFTF OC4R 


#define FORWARD 1 
#define BACKWARDS 2 
#define RIGHT 3 
#define LEFT 4 


#define FORWARD 1 
#define BACKWARDS 2 
#define RIGHT 3 
#define LEFT 4 


#define PWMFREQUENCY 7200

//defining structs
typedef struct {
    char buffer[BUFFER_SIZE];
    int head;
    int tail;
    int count;
} CircularBuffer;



// Function prototypes
void tmr_setup_period(int8_t timer, int ms);
int tmr_wait_period(int8_t timer);
void tmr_wait_ms(int8_t timer, int ms);
void init_UART1();
void print_UART1(unsigned char msg);
void print_buffer_UART1(char buffer[]);
void init_SPI1();
void write_SPI1(unsigned char addr, unsigned char msg);
unsigned char read_SPI1(unsigned char addr);
void setup_mag();
int16_t mag_get_x();
int16_t mag_get_y();
int16_t mag_get_z();
void initCircularBuffer(CircularBuffer *cb);
int isEmpty(CircularBuffer *cb);
int isFull(CircularBuffer *cb);
void enqueue(CircularBuffer *cb, char value);
int dequeue(CircularBuffer *cb);

void init_adc();

void set_up_PWM_wheels();
void move(int dir, int speed);
void stop_moving();

#endif	/* XC_HEADER_TEMPLATE_H */
