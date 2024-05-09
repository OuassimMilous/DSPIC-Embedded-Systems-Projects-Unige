/*
 * File:   newmainXC16.c
 * Author: YNS
 *
 * Created on May 3, 2024, 4:27 PM
 */


#include "xc.h"
#include"tools.h"
#include <stdio.h>
#include <math.h>

char msg[25];

#define BUFFER_SIZE 25


int i = 0;
int j = 0;
int jj = 0;
int angle;
//nt c[7] = {1, 2, 3, 4, 5, 6, 7};
int recx = 0, recy = 0, recz = 0;



//////////////////////////////////////

typedef struct {
    char buffer[BUFFER_SIZE];
    int head;
    int tail;
    int count;
} CircularBuffer;

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

char dequeue(CircularBuffer *cb) {
    if (isEmpty(cb)) {
        return -1; // Assuming -1 represents an error condition
    }
    int value = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    cb->count--;
    return value;
}


CircularBuffer cb;

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////





//int main(void) {
//    int x = 0;
//    int recx=0, recy=0, recz=0;
//    tmr_setup_period(TIMER1, 10);
//    while (1) {
//        tmr_wait_ms(TIMER2, 7);
//        init_SPI1();
//        init_UART1();
//        setup_mag();
//        recx = recx+mag_get_x();
//        recy=recy+mag_get_y();
//        recz=recz+mag_get_z();
//        x++;
//
//        if (x == 5) {
//            x=0;
//            recx=recx/5;
//            recy=recy/5;
//            recz=recz/5;
//            
//            
//
//
//
//        }
//
//
//
//
//
//
//
//    }
//
//
//
//
//    return 0;
//}

void __attribute__((__interrupt__, __auto_psv__))_U1TXInterrupt() {

    IFS0bits.U1TXIF = 0;
    int data = dequeue(&cb);
    if (data != -1) {
        U1TXREG = data;
    }
}

int main(void) {

    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    initCircularBuffer(&cb);

    //enqueue(&cb, 1);
    //U1TXREG = dequeue(&cb);


    TRISAbits.TRISA0 = 0;
    LATAbits.LATA0 = 0;
    init_SPI1();

    setup_mag();

    U1STAbits.UTXISEL0 = 0b0;
    U1STAbits.UTXISEL1 = 0b0;
    init_UART1();
    IEC0bits.U1TXIE = 1;


   














    tmr_setup_period(TIMER1, 10);



    while (1) {
        tmr_wait_ms(TIMER2, 7);

        while (j == 4) {
           
            recx = mag_get_x() / 5 + recx;
            recy = mag_get_y() / 5 + recy;
            recz = mag_get_z() / 5 + recz;
            j = 0;
        }
      





        if (jj == 20) {

            //                    recx = recx / 5;
            //                    recy = recy / 5;
            //                    recz = recz / 5;
            //IEC0bits.U1TXIE = 0;

            //            
            //           
            //
            //                            enqueue(&cb,recx);
            //                            // U1TXREG = dequeue(&cb);
            //                            enqueue(&cb, recy);
            //                            //U1TXREG = dequeue(&cb);
            //                            enqueue(&cb,recz);
            //                            enqueue(&cb, 2);
            //                            enqueue(&cb, 2);
            //            enqueue(&cb, 2);
            //
            //            

            // U1TXREG = '$';


            sprintf(msg, "$MAG,%d,%d,%d*, $YAW,%f* ", recx, recz, recy, atan2(recx, recy) * 180.0 / M_PI);
            int c = 0;
            do {
                enqueue(&cb, msg[c]);
                c++;
            } while (msg[c ] != '\0');
            enqueue(&cb, ' ');
            U1TXREG = dequeue(&cb);

            //           U1TXREG = 0x2A;

            jj = 0;

            // LATAbits.LATA0 = !LATAbits.LATA0;

            recx = 0;
            recy = 0;
            recz = 0;

        }










        LATAbits.LATA0 = tmr_wait_period(TIMER1);
        j++;
        jj++;


    }
    //        write_SPI1(0x4C, 0b10000110);
    //        unsigned char x = 0b11111111;
    //        unsigned char y = 0b11111111;
    //        int z = join_msb_lsb(y, x);
    //        int num = 123;
    //        char str[3]; // Need space for 3 digits + 1 for the null terminator
    //        sprintf(str, "%d", z);
    //        unsigned char ccc=0x33;
    //    int magx;
    //    while (1) {
    //
    //        tmr_setup_period(TIMER2, 100);
    //
    //        magx = mag_get_x();
    //
    //        U1TXREG = magx;
    //
    //        int ret = tmr_wait_period(TIMER2);
    //
    //
    //    }

    return 0;
}