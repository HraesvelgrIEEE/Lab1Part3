/*
* File:   leds.c
* Author: 
*
* Created on December 27, 2014, 1:31 PM
*/

#include <xc.h>
#include "led.h"

#define OUTPUT 0
#define OFF 0

void initLEDs(){
    TRISDbits.TRISD2 = OUTPUT;    
    TRISDbits.TRISD0 = OUTPUT;
    
    LATDbits.LATD2 = OFF;
    LATDbits.LATD0 = OFF;
}