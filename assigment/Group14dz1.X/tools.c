#include "xc.h"
#include "tools.h"

// a function to set up a period for a timer
void tmr_setup_period(int8_t timer, int ms) {
    //selecting the timer
    switch (timer) {
        case 1:
            //resetting the timer's register
            TMR1 = 0;
            //setting up the prescaler
            PR1 = (ms / 1000.0)*281250;
            // selecting the 1:256 prescaler
            T1CONbits.TCKPS = 0b11;
            // enabling the timer
            T1CONbits.TON = 1;
            break;

        case 2:
            //resetting the timer's register
            TMR2 = 0;
            //setting up the prescaler
            PR2 = (ms / 1000.0)*281250;
            // selecting the 1:256 prescaler
            T2CONbits.TCKPS = 0b11;
            // enabling the timer
            T2CONbits.TON = 1;
            break;
    }
}

// a function that waits until the set period has fully passed
int tmr_wait_period(int8_t timer) {
    
    // declaring a variable to check if the deadline is missed or not
    int isNotMissed;
    //selecting the timer
    switch (timer) {
        case 1:
            //checking the deadline
            isNotMissed = IFS0bits.T1IF;
            //waiting untill the period ends
            while (!IFS0bits.T1IF);
            //resetting the flag again
            IFS0bits.T1IF = 0;
            break;

        case 2:
            //checking the deadline
            isNotMissed = IFS0bits.T2IF;
            //waiting untill the period ends
            while (!IFS0bits.T2IF);
            //resetting the flag again
            IFS0bits.T2IF = 0;
            break;
    }
    
    //returning the deadline status
    return isNotMissed;
}

// a wait function based on timers
void tmr_wait_ms(int8_t timer, int ms) {
    
    // calculating the number of ticks using unsigned long long to support larger values
    unsigned long long ticks = (ms / 1000.0)*281250; 

    // Configure the timer and its period register accordingly
    switch (timer) {
        case 1:
            // Disable Timer1 during configuration
            T1CONbits.TON = 0;
            // Set prescaler to 1:256
            T1CONbits.TCKPS = 0b11;
            // Clear Timer1 value
            TMR1 = 0;

            // Adjust the period register and handle overflow
            while (ticks > 65535) {
                ticks -= 65535;
                // Set maximum value for Timer1 period
                PR1 = 65535;
                // Enable Timer1
                T1CONbits.TON = 1;
                // Wait for Timer1 period to complete
                tmr_wait_period(TIMER1);
            }
            // Disable Timer1
            T1CONbits.TON = 0;
            // Set remaining ticks as period register
            PR1 = (unsigned int) ticks;
            // Enable Timer1
            T1CONbits.TON = 1;
            // Wait for Timer1 period to complete one last time
            tmr_wait_period(TIMER1);
            break;
            
        case 2:
            // Disable Timer2 during configuration
            T2CONbits.TON = 0;
            // Set prescaler to 1:256
            T2CONbits.TCKPS = 0b11;
            // Clear Timer1 value
            TMR2 = 0;

            // Adjust the period register and handle overflow
            while (ticks > 65535) {
                ticks -= 65535;
                // Set maximum value for Timer1 period
                PR2 = 65535;
                // Enable Timer2
                T2CONbits.TON = 1;
                // Wait for Timer2 period to complete
                tmr_wait_period(TIMER2);
            }
            // Disable Timer1
            T2CONbits.TON = 0;
            // Set remaining ticks as period register
            PR2 = (unsigned int) ticks;
            // Enable Timer2
            T2CONbits.TON = 1;
            // Wait for Timer2 period to complete one last time
            tmr_wait_period(TIMER2);
            break;
    }
}

// initializing the SPI
void init_SPI1() {
    //configuring the SPI
    SPI1CON1bits.MSTEN = 1; // master mode
    SPI1CON1bits.MODE16 = 0; // 8?bit mode
    SPI1CON1bits.PPRE = 0; // 1:1 primary prescaler
    SPI1CON1bits.SPRE = 5; // 5:1 secondary prescaler
    SPI1CON1bits.CKP = 1; // setting the idle bit as 1
    SPI1STATbits.SPIEN = 1; // enable SPI

    //setting up pins
    TRISAbits.TRISA1 = 1; // RA1?RPI17 MISO
    TRISFbits.TRISF12 = 0; //RF12?RP108 SCK
    TRISFbits.TRISF13 = 0; // RF13?RP109 MOSI

    //reamapping the pins
    RPINR20bits.SDI1R = 0b0010001; // MISO (SDI1) ? RPI17
    RPOR12bits.RP109R = 0b000101; // MOSI (SDO1) ? RF13
    RPOR11bits.RP108R = 0b000110; // SCK1;
    
    //setting up chip select pins and making sure theyre not selected
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISDbits.TRISD6 = 0;

    LATBbits.LATB3 = 1;
    LATBbits.LATB4 = 1;
    LATDbits.LATD6 = 1;
}

// initializing the UART
void init_UART1() {
    // remap input pin U1RXR to RD11
    TRISDbits.TRISD11 = 1;
    RPINR18bits.U1RXR = 75;
    // remap output pin U1TX to RD0
    TRISDbits.TRISD0 = 0;
    RPOR0bits.RP64R = 1;

    //configuring the UART
    U1BRG = 468; // 72M / (16 * 9600) - 1
    U1MODEbits.UARTEN = 1; // enable UART
    U1STAbits.UTXEN = 1;

}

// a function to print one character through UART
void print_UART1(unsigned char msg) {
    U1TXREG = msg;
}

// a function to print multiple character through UART
void print_buffer_UART1(char buffer[]) {
    int j = 0;
    while (buffer[j] != '\0') {
        print_UART1(buffer[j]);
        j++;
    }
}

// a function to recieve one char from SPI
static unsigned char recieve_SPI1() {
    //waits until the reading buffer is available
    while (SPI1STATbits.SPIRBF == 0);
    //reads then returns the read data
    unsigned char data = SPI1BUF;
    return data;
}

// a function to transmit one char through SPI
static unsigned char transmit_SPI1(unsigned char msg) {
    //waits until the writing buffer is available
    while (SPI1STATbits.SPITBF == 1);
    //sends the data
    SPI1BUF = msg;
    //read the resulting in case were reading, or just to avoid overrun
    unsigned char data = recieve_SPI1();
    return data;
}

// a function that writes specific data ona specific address
void write_SPI1(unsigned char addr, unsigned char msg) {
    // enabling the chip select
    CS = 0;
    //transmit the address then the message
    transmit_SPI1(addr);
    transmit_SPI1(msg);
    // disabling the chip select again
    CS = 1;
}

// a function that writes specific data ona specific address
unsigned char read_SPI1(unsigned char addr) {
    // enabling the chip select
    CS = 0;
    //transmit the address then recieve the message
    transmit_SPI1(addr | 0x80); // force msb to 1
    unsigned char reced = transmit_SPI1(0x00);
    // disabling the chip select again
    CS = 1;
    //returning the data
    return reced;
}

// setting up the magnetometer
void setup_mag() {
    //sleep mode
    write_SPI1(MAGSLEEPMODE, 0x01);

    //sleep for 2 ms
    tmr_wait_ms(TIMER2, 2);

    //active mode
    write_SPI1(MAGOPMODE, 0x00);
}

// a function that joins two characters
int16_t join_msb_lsb(unsigned char lsb, unsigned char msb) {
    return (msb << 8) | lsb;
}

// a function that gets the x axis value
int16_t mag_get_x() {
    // read the lsb and the msb from the sensor
    int16_t lsb = read_SPI1(MAGXLSB);
    int16_t msb = read_SPI1(MAGXMSB);

    lsb = (lsb & 0b11111000); // masking
    int16_t x = join_msb_lsb(lsb, msb); // merging the lsb and the msb
    x /= 8; // deleting the first unnecessary first 0s
    
    //fixing signs and returning the data
    return x;
}

// a function that gets the y axis value
int16_t mag_get_y() {
        // read the lsb and the msb from the sensor
    int16_t lsb = read_SPI1(MAGYLSB);
    int16_t msb = read_SPI1(MAGYMSB);
    lsb = (lsb & 0b11111000); // masking
    int16_t y = join_msb_lsb(lsb, msb); // merging the lsb and the msb
    y /= 8;  // deleting the first unnecessary first 0s

    //fixing signs and returning the data
    return y;
}

// a function that gets the z axis value
int16_t mag_get_z() {
    int16_t lsb = read_SPI1(MAGZLSB);
    int16_t msb = read_SPI1(MAGZMSB);
    lsb = (lsb & 0b11111110); // masking
    int16_t z = join_msb_lsb(lsb, msb); // merging the lsb and the msb
    z /= 2; // deleting the first unnecessary first 0s

    //fixing signs and returning the data
    return z;
}

// Initialize the circular buffer
void initCircularBuffer(CircularBuffer *cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

// Check if the circular buffer is empty
int isEmpty(CircularBuffer *cb) {
    return (cb->count == 0);
}

// Check if the circular buffer is full
int isFull(CircularBuffer *cb) {
    return (cb->count == BUFFER_SIZE);
}

// Enqueue an element into the circular buffer
void enqueue(CircularBuffer *cb, char value) {
    if (isFull(cb)) {
        return;
    }
    cb->buffer[cb->head] = value;
    cb->head = (cb->head + 1) % BUFFER_SIZE;
    cb->count++;
}

// Dequeue an element from the circular buffer
char dequeue(CircularBuffer *cb, char *value) {
    if (isEmpty(cb)) {
        return 0; // Assuming 0 represents an error condition
    }
    value = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    cb->count--;
    
    return 1; // Assuming 1 represents a successful condition
}
