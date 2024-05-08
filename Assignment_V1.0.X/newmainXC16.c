/*
 * File:   newmainXC16.c
 * Author: YNS
 *
 * Created on May 3, 2024, 4:27 PM
 */


#include "xc.h"
#include"tools.h"

#define BUFFER_SIZE 11


int i = 0;
int j = 0;
int jj = 0;
int c[7] = {1, 2, 3, 4, 5, 6, 7};
int recx, recy, recz;



//////////////////////////////////////

typedef struct {
    int buffer[BUFFER_SIZE];
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

void enqueue(CircularBuffer *cb, int value) {
    if (isFull(cb)) {


        return;
    }
    cb->buffer[cb->head] = value;
    cb->head = (cb->head + 1) % BUFFER_SIZE;
    cb->count++;
}

// Dequeue an element from the circular buffer

int dequeue(CircularBuffer *cb) {
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


    initCircularBuffer(&cb);

    //enqueue(&cb, 1);
    //U1TXREG = dequeue(&cb);


    TRISAbits.TRISA0 = 0;
    LATAbits.LATA0 = 0;
    init_SPI1();
    TRISDbits.TRISD11 = 1;
    RPINR18bits.U1RXR = 75;
    // remap output pin U1TX to RD0
    TRISDbits.TRISD0 = 0;
    RPOR0bits.RP64R = 1;

    // select Interrupt

    U1STAbits.UTXISEL0 = 0b0;
    U1STAbits.UTXISEL1 = 0b0;
    // enqueue(&cb, 5);




    //configuring the UART
    U1BRG = 468; // 72M / (16 * 9600) - 1
    U1MODEbits.UARTEN = 1; // enable UART
    U1STAbits.UTXEN = 1;
    IEC0bits.U1TXIE = 1;
    // IFS0bits.U1TXIF = 1;
    //
    //    U1TXREG = 1;
    //    U1TXREG = 1;
    //    U1TXREG = 1;
    //    U1TXREG = 1;








    tmr_setup_period(TIMER1, 10);


    j = 0;
    while (1) {
        tmr_wait_ms(TIMER2, 7);

        //        while (j = 4) {
        //
        //            recx = recx + mag_get_x();
        //            recy = recy + mag_get_y();
        //            recz = recz + mag_get_z();
        //
        //            j = 0;
        //        }
        //
        //        recx = recx / 5;
        //        recy = recy / 5;
        //        recz = recz / 5;

        //
        //        jj = 0;


        if (jj == 20) {
            //IEC0bits.U1TXIE = 0;
            enqueue(&cb, 1);
            U1TXREG = dequeue(&cb);
         
            enqueue(&cb, 6);
            // U1TXREG = dequeue(&cb);
               enqueue(&cb, 6);
            //U1TXREG = dequeue(&cb);
            enqueue(&cb, 6);
            enqueue(&cb, 6);
            enqueue(&cb, 6);
            enqueue(&cb, 6);


            //U1TXREG = dequeue(&cb);
            jj = 0;

            // LATAbits.LATA0 = !LATAbits.LATA0;
        }
       









        tmr_wait_period(TIMER1);
        j = 1;
        jj++;


    }




    return 0;
}