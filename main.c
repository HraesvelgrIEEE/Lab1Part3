/* File: main.c
 * Author: dhung 
 */

//#define TESTING

#include <xc.h>
#include <sys/attribs.h>
#include "timer.h"
#include "led.h"
#include "lcd.h"
#include "timer.h"
#include "config.h"
#include "interrupt.h"

//#define RUN_LED  LATGbits.LATG12
//#define STOP_LED  LATGbits.LATG14
#define RUN_BUTTON PORTAbits.RA7
#define RESET_BUTTON PORTGbits.RG13

#define ON 1
#define OFF 0
#define PRESSED 0
#define UNPRESSED 1
#define STOP 1
#define START 0

#ifdef TESTING
#include "tests.h"
#endif

// ******************************************************************************************* //
typedef enum stateTypeEnum {run, stop, debounce, waitRelease, reset} stateType;

volatile unsigned timeElapsed = 0; //In milliseconds
volatile stateType state = stop, nextState = stop;

int main(void)
{
    //Initialize
    initLEDs();
    initSwitches();
    initLCD();
    initTimers();
    enableInterrupts();
    SYSTEMConfigPerformance(10000000);
    
#ifdef TESTING
    while(1) {
        writeLCDTest();
    }
#else
    //Main loop
    while(1)
    {
        switch (state) {
            case stop:
                SwitchLEDs(STOP);
                printLineLCD("Stopped",1); // string, line#
                printTimeLCD(timeElapsed); // converts time and prints on line 2 of LCD
                break;
            case run:
                SwitchLEDs(START);
                printLineLCD("Running", 1); // string, line#
                printTimeLCD(timeElapsed); // converts time and prints on line 2 of LCD
                break;
            case debounce:
                delayMilliseconds(5);
                state = waitRelease;
                break;
            case waitRelease:
                delayMilliseconds(5);
                break;
            case reset:
                timeElapsed = 0;
                state = stop;
                break;
        }
    }
#endif
    
    return 0;
}

__ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _StopStartInterrupt() { // STOP/START
    IFS1bits.CNAIF = 0;
    IFS1bits.CNGIF = 0;
    if (RESET_BUTTON == PRESSED) {
        if (state == stop){
            state = debounce;
            nextState = reset;
        }
    }
    else if (RUN_BUTTON == PRESSED) {
        switch (state) {
            case run:
                nextState = stop;
                break;
            case stop:
                nextState = run;
                break;
        }
        state = debounce;
    }
    else {
        if (state = waitRelease) {
            state = nextState;
        }
    }
}

__ISR(_TIMER_1_VECTOR, IPL7SRS) _T1Interrupt() {
    IFS0bits.T1IF = 0;
    
    if (state == run) {
        timeElapsed += 10; //Increment time by 10 ms
    }
}