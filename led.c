/*
* File:   leds.c
* Author: 
*
* Created on December 27, 2014, 1:31 PM
*/

#include <xc.h>
#include "led.h"

#define ON 1
#define OFF 0
#define STOP 1
#define START 0
#define OUTPUT 0

void initLEDs(){
    TRISGbits.TRISG12 = OUTPUT;
    TRISGbits.TRISG14 = OUTPUT;
    
    LATGbits.LATG12 = ON;
    LATGbits.LATG14 = ON;
}


void SwitchLEDs(int led){
    if(led == START){
        LATGbits.LATG12 = ON;
        LATGbits.LATG14 = OFF;
    }
    else if(led == STOP){
        LATGbits.LATG12 = OFF;
        LATGbits.LATG14 = ON;
    }
}