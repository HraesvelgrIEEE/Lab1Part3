/*
 * File:   switch.c
 * Author: Garrett
 *
 * Created on September 19, 2015, 10:46 AM
 */

#include <xc.h>
#include "switch.h"

#define INPUT 1
#define ENABLED 1

void initSwitches(){
    //Run switch
    TRISDbits.TRISD1 = INPUT;
    CNENDbits.CNIED1 = ENABLED;
    CNPUDbits.CNPUD1 = ENABLED;
    
    //Reset switch
    TRISDbits.TRISD3 = INPUT; //FIXME
    CNENDbits.CNIED3 = ENABLED;
    CNPUDbits.CNPUD3 = ENABLED;
    
    //Interrupts
    CNCONDbits.ON = ENABLED;
    IFS1bits.CNDIF = 0; //lower flag
    IPC8bits.CNIP = 7; //default priority
    IEC1bits.CNDIE = ENABLED;
}
