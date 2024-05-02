#include "xc.h"
#include "tools.h"

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

void init_SPI1(){
    //configuring the SPI
    SPI1CON1bits.MSTEN = 1; // master mode
    SPI1CON1bits.MODE16 = 0; // 8?bit mode
    SPI1CON1bits.PPRE = 0; // 1:1 primary prescaler
    SPI1CON1bits.SPRE = 5; // 5:1 secondary prescaler
    SPI1CON1bits.CKP = 1;
    SPI1STATbits.SPIEN = 1; // enable SPI
 
    //setting up the CS and making sure theyre not selected
    TRISAbits.TRISA1 = 1; // RA1?RPI17 MISO
    TRISFbits.TRISF12 = 0; //RF12?RP108 SCK
    TRISFbits.TRISF13 = 0; // RF13?RP109 MOSI

    
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISDbits.TRISD6 = 0;

    LATBbits.LATB3 = 1;
    LATBbits.LATB4 = 1;
    LATDbits.LATD6 = 1;

    //reamapping the pins
    RPINR20bits.SDI1R = 0b0010001; // MISO (SDI1) ? RPI17
    RPOR12bits.RP109R = 0b000101; // MOSI (SDO1) ? RF13
    RPOR11bits.RP108R = 0b000110; // SCK1;

}

void init_UART1(){
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

void print_UART1(unsigned char msg){
//    while(!U1STA.TRMT)
    U1TXREG = msg;
}

void print_buffer_UART1(char buffer[]) {
    int j = 0;
    while (buffer[j] != '\0') {
        U1TXREG = buffer[j];
        j++;
    }
}


static unsigned char recieve_SPI1() {
    while (SPI1STATbits.SPIRBF == 0);
    unsigned char data = SPI1BUF; 
    
    return data;
}

static unsigned char transmit_SPI1(unsigned char msg) {
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = msg;
    //avoid overrun
    unsigned char trash = recieve_SPI1(); 
    return trash;
}

void write_SPI1(unsigned char addr, unsigned char msg) {  
    CS = 0; 
    transmit_SPI1(addr); // force msb to be 0
    transmit_SPI1(msg);
    CS = 1;
}


unsigned char read_SPI1 (unsigned char addr) {
    CS = 0; 
    transmit_SPI1(addr | 0x80); // force msb to 1
     unsigned char reced  = transmit_SPI1(0x00);
    CS = 1;
    return reced;
}

void setup_mag(){
    
    //sleep mode
    write_SPI1(MAGSLEEPMODE, 0x01);

    //sleep for 2 ms
    tmr_wait_ms(TIMER1,2);
    
    //active mode
    write_SPI1(MAGOPMODE,0x00);
}

int join_msb_lsb(unsigned char lsb, unsigned char msb){
    return (msb << 8) | lsb;
}

int mag_get_x(){
    int lsb = read_SPI1(MAGXLSB);
    int msb = read_SPI1(MAGXMSB);
   
    lsb = (lsb & 0b11111000) ;// masking
    int x = join_msb_lsb(lsb,msb); // merging the lsb and the msb
    x /=8;
    return x;
}

int mag_get_y(){
    int lsb = read_SPI1(MAGYLSB);
    int msb = read_SPI1(MAGYMSB);
    lsb = (lsb & 0b11111000) ;// masking
    int y = join_msb_lsb(lsb,msb); // merging the lsb and the msb
    y /= 8; // removing

    return y;
}
int mag_get_z(){
    int lsb = read_SPI1(MAGYLSB);
    int msb = read_SPI1(MAGYMSB);
    lsb = (lsb & 0b11111000) ;// masking
    int z = join_msb_lsb(lsb,msb); // merging the lsb and the msb
    z /= 2; // removing

    return z;
}
