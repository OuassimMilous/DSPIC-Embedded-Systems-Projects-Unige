#include "xc.h"
#include "tools.h"
#include <stdio.h>

int chip_id;
int magx;


int main(void) {
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    init_SPI1(); 

    // configuring the UART
    init_UART1(); 

    // configuring the SPI
    setup_mag();

    // read ID
    chip_id = read_SPI1(0x40);
   
    // printing the received value
    print_UART1(chip_id);
    
   

    while (1){
        
        tmr_setup_period(TIMER2,100);
        
        magx = mag_get_x();

        print_buffer_UART1(magx);

        int ret = tmr_wait_period(TIMER1);    
    }

    return 0;
}

