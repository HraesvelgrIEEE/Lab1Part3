/* File: tests.c
 * Author: dhung 
 */

#include "tests.h"
#include "lcd.h"
#include "timer.h"
#include <xc.h>
#include <string.h>

#define LED1 LATDbits.LATD0 
#define LED2 LATDbits.LATD1 

#define OUTPUT 0
#define HIGH 1
#define LOW 0

void initTestLEDs() {
    TRISDbits.TRISD0 = OUTPUT;
    TRISDbits.TRISD1 = OUTPUT;
    
    LED1 = LOW;
    LED2 = LOW;
}

void testDelayUs() {
    initTestLEDs();
    
    //1 second delay, verify externally
    int i = 0;
    int j = 0;
    
    LED1 = HIGH; //Init one high
    
    //Loop for 30 seconds
    for (i = 0; i != 30; ++i) {
        for (j = 0; j != 1000; ++j) delayUs(1000); //1 millisecond delay
        LED1 = !LED1;
        LED2 = !LED2; //Toggle
    }
    
    //This should tick each second
}

void testLCD(){
    initLCD();
    int i = 0;
    printCharLCD('c');
    for(i = 0; i < 1000; i++) delayUs(1000);
    clearLCD();
    printStringLCD("Hello!");
    moveCursorLCD(1, 2);
    for(i = 0; i < 1000; i++) delayUs(1000);
    printStringLCD("Hello!");
    for(i = 0; i < 1000; i++) delayUs(1000);
}

void printTimeLCDTest() {
    initTestLEDs();
    
    char correctTimeString[9];
    char testTimeString[9];
    
    testTimeString = printTimeLCD(75030);
    
    correctTimeString[0] = '0';
    correctTimeString[1] = '1';
    correctTimeString[2] = ':';
    correctTimeString[3] = '1';
    correctTimeString[4] = '5';
    correctTimeString[5] = ':';
    correctTimeString[6] = '0';
    correctTimeString[7] = '3';
    correctTimeString[8] = '\0'; //Terminate
    
    if (strcmp(testTimeString, correctTimeString) == 0) {
        //Strings equal, test passed
        LED1 = HIGH;
        LED2 = LOW; 
    }
    else {
        //Test failed
        LED1 = LOW;
        LED2 = HIGH;
    }
}
