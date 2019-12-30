/*
 * File:   main.c
 * Author: chris
 *
 * Created on December 23, 2019, 12:30 PM
 */


#include "xc.h"

int main(void) {
    
    // Configure LED pin (RE0) as output
    TRISEbits.TRISE0 = 0;
    
    while (1)
    {
        // Toggle LED pin.
        PORTEbits.RE0 = !PORTEbits.RE0;
        
        // Wait.
        for (volatile long i = 0; i < 100000; i ++);
    }
    
    return 0;
}
