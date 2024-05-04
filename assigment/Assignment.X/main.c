#include "xc.h"
#include "tools.h"
    int x,y,z,sumx = 0, sumy = 0, sumz = 0,i=0;
    int xarr[5];
    int yarr[5];
    int zarr[5];
int main(void){
    
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    

    //initializing spi
    init_SPI1(); 
    
    // configuring the UART
    init_UART1(); 

    // configuring the SPI
    setup_mag();
    
    
    print_UART1(0x56);

    tmr_setup_period(TIMER1,40);
    tmr_setup_period(TIMER2,10);
    
    while(1){

            tmr_wait_ms(TIMER3,7);
            
            if(tmr_check_period(TIMER1)){
                xarr[i] = mag_get_x();
                yarr[i] = mag_get_y();
                zarr[i] = mag_get_z();
                i==5? i=0: i++;
            }
            
            if(i==5){
                sumx = 0;
                sumy = 0;
                sumz = 0;
                for(int j = 0; j<5; j++){
                    sumx+= xarr[i];
                    sumy+= yarr[i];
                    sumz+= zarr[i];
                }
                x = sumx/5;
                y = sumy/5;
                z = sumz/5;

                print_UART1(0x00);

                print_UART1(x);
                print_UART1(y);
                print_UART1(z);

                print_UART1(0x00);


            }
            tmr_wait_period(TIMER2);
    }

    return 0;
}
