#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> 

//defining constants
#define TIMER1 1
#define TIMER2 2
#define CS LATDbits.LATD6
#define BUFFER_SIZE 37
#define BUFFER_SIZE_CMD 10

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

#define PWMFREQUENCY 7200

#define STATE_DOLLAR  (1) // we discard everything until a dollar is found
#define STATE_TYPE    (2) // we are reading the type of msg until a comma is found
#define STATE_PAYLOAD (3) // we read the payload until an asterix is found

#define NEW_MESSAGE (1) // new message received and parsed completely
#define NO_MESSAGE (0) // no new messages

typedef struct { 
	int state;
	char msg_type[6]; // type is 5 chars + string terminator
	char msg_payload[100];  // assume payload cannot be longer than 100 chars
	int index_type;
	int index_payload;
} parser_state;


        
//defining structs
typedef struct {
    char buffer[BUFFER_SIZE];
    int head;
    int tail;
    int count;
} CircularBuffer;


//typedef struct {
//    parser_state buffer[BUFFER_SIZE_CMD];
//    int head;
//    int tail;
//    int count;
//} CircularBufferCMD;



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



/*
Requires a pointer to a parser state, and the byte to process.
returns NEW_MESSAGE if a message has been successfully parsed.
The result can be found in msg_type and msg_payload.
Parsing another byte will override the contents of those arrays.
*/
int parse_byte(parser_state* ps, char byte);

/*
Takes a string as input, and converts it to an integer. Stops parsing when reaching
the end of string or a ","
the result is undefined if a wrong string is passed
*/
int extract_integer(const char* str);

/*
The function takes a string, and an index within the string, and returns the index where the next data can be found
Example: with the string "10,20,30", and i=0 it will return 3. With the same string and i=3, it will return 6.
*/  
int next_value(const char* msg, int i);


#endif	/* XC_HEADER_TEMPLATE_H */
