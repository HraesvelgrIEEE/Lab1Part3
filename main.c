/* File: main.c
 * Author: dhung 
 */

#include <xc.h>
#include <sys/attribs.h>
#include "timer.h"
#include "led.h"
#include "lcd.h"
#include "timer.h"
#include "config.h"
#include "interrupt.h"

#define RUN_LED LATDbits.LATD2
#define STOP_LED LATDbits.LATD0
#define RUN_BUTTON PORTDbits.RD1;
#define RESET_BUTTON PORTDbits.RD3

#define ON 1
#define OFF 0
#define PRESSED 0
#define UNPRESSED 1

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
    
    SYSTEMConfigPerformance(10000000);
    
    //Main loop
    while(1)
    {
        //TODO: Stuff
        switch (state) {
            case stop:
                RUN_LED = OFF;
                STOP_LED = ON;
                //set LCD to line 1
                printStringLCD("Stopped");
                //set LCD to line 2
                printTimeLCD(timeElapsed);
                break;
            case run:
                RUN_LED = ON;
                STOP_LED = OFF;
                //Set LCD to line 1
                printStringLCD("Running");
                //Set LCD to line 2
                printTimeLCD(timeElapsed);
                break;
            case debounce:
                delayMilliseconds(5);
                state = waitRelease;
                break;
            case waitRelease:
                break;
            case reset:
                timeElapsed = 0;
                state = stop;
                break;
        }
    }
    
    return 0;
}

__ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt() {
    IFS1bits.CNDIF = 0;
    
    PORTDbits;
    
    //Reset holds priority
    if (RESET_BUTTON == PRESSED) {
        state = debounce;
        nextState = reset;
    }
    
    else {
        switch (state) {
            case run:
                //check button
                if (RUN_BUTTON == PRESSED) {
                    state = debounce;
                    nextState = stop;
                }
                break;
            case stop:
                //check button
                if (RUN_BUTTON == PRESSED) {
                    state = debounce;
                    nextState = run;
                }
                break;
            case waitRelease:
                if (RUN_BUTTON == UNPRESSED && RESET_BUTTON == UNPRESSED) {
                    state = nextState;
                }
                break;
        }
    }
}

__ISR(_TIMER_1_VECTOR, IPL7SRS) _T1Interrupt() {
    IFS0bits.T1IF = 0;
    
    if (state == run) {
        timeElapsed += 10; //Increment time by 10 ms
    }
}