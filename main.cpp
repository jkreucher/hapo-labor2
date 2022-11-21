/* HAPO Labor 2
Gruppe:
    Jannik Kreucher
    Benedikt Wendling
*/
#include "mbed.h"
#include <cstdint>
#include <stdint.h>

// Nucleo On-Board
#define PIN_NUC_LED PA_5
#define PIN_NUC_SW  PC_13
// Shield
#define PIN_SW1     PA_1
#define PIN_SW2     PA_4
#define PIN_SW3     PB_0
#define PIN_DOUT    PA_9
#define PIN_SCLK    PA_8
#define PIN_LTCH    PB_5
#define PIN_LED1    PB_6
#define PIN_LED2    PA_7
#define PIN_LED3    PA_6
#define PIN_LED4    PA_5
#define PIN_BUZZER  PB_3


// 7 segment display data
//  0bHGFEDCBA active low
const uint16_t segment_data[17] = {
    //HGFEDCBAxxxx1234
    0b1100000000000000, // 0
    0b1111100100000000, // 1
    0b1010010000000000, // 2
    0b1011000000000000, // 3
    0b1001100100000000, // 4
    0b1001001000000000, // 5
    0b1000001000000000, // 6
    0b1111100000000000, // 7
    0b1000000000000000, // 8
    0b1001000000000000, // 9
    0b1000100000000000, // A
    0b1000001100000000, // B
    0b1100011000000000, // C
    0b1010000100000000, // D
    0b1000011000000000, // E
    0b1000111000000000, // F
    0b1111111100000000, // Off
};
// 7 segment display digits
const uint16_t segment_digit[4] = {
    //xxxx1234
    0b00001000,
    0b00000100,
    0b00000010,
    0b00000001
};

enum eDisplayStates {
    StateOff,
    StateDots,
    StateCount,
    StateHold
};

enum eMainStates {
    StateWaiting,
    StateCounting,
    StatePause
};

// global vars
volatile uint8_t bMainUpdate = 0;

uint8_t bDisplayState = StateOff; // current display state
uint8_t bDisplayDigit = 0;   // current digit to update
uint8_t bDisplayPoint = 4;   // decimal point position
uint8_t bDisplayCounter = 0; // for display timing
uint16_t wDisplayTime = 0;   // elapsed time in 10ms
uint8_t bDisplayValues[4] = {16, 16, 16, 16}; // display values for each digit

uint8_t bMainState = StateWaiting; // main state machine state
uint16_t wWaitTime = 1200;  // time to wait for counting
uint16_t wPauseCounter = 0; // result duration
uint8_t bButtonToPress = 0; // button id to press

uint16_t wBuzzerTimer = 0; // buzzer timer

uint16_t wRandomNumber = 0; // random number gets incremented


Ticker tMainUpdate;


// Input / Output
BusOut busLeds(PIN_LED1, PIN_LED2, PIN_LED3, PIN_LED4);
DigitalIn pinSW1(PIN_SW1);
DigitalIn pinSW2(PIN_SW2);
DigitalIn pinSW3(PIN_SW3);
DigitalOut pinDOUT(PIN_DOUT);
DigitalOut pinSCLK(PIN_SCLK);
DigitalOut pinLTCH(PIN_LTCH);
DigitalOut pinBuzzer(PIN_BUZZER);


// Prototypes
void vShiftOut(uint16_t wData);
void vMainUpdate();
void vDisplayStateMachine();
uint8_t bCheckButton();


// main() runs in its own thread in the OS
int main() {
    tMainUpdate.attach(&vMainUpdate, 2500us);

    // initial state
    busLeds = 0xF;
    bMainState = StateWaiting;

    while(1) {
        // check main state machine
        switch(bMainState) {
            case StateWaiting:
                // set current display state
                bDisplayState = StateDots;
                if(!wWaitTime) {
                    // reset display time
                    wDisplayTime = 0;
                    // enable buzzer for 50ms
                    wBuzzerTimer = 20;
                    // random button number
                    bButtonToPress = wRandomNumber % 3;
                    // next state
                    bMainState = StateCounting;
                }
                wWaitTime--;
                break;
            
            case StateCounting:
                // set display state
                bDisplayState = StateCount;
                // show button number on leds
                busLeds = ~(1 << bButtonToPress);
                // check buttons
                if((bCheckButton() != 3) || (wDisplayTime > 999)) {
                    // button has been pressed
                    if((bCheckButton() != bButtonToPress) || (wDisplayTime > 999)) {
                        // sound buzzer
                        wBuzzerTimer = 200;
                        // error on display
                        bDisplayValues[0] = 0xF;
                        bDisplayValues[1] = 0xF;
                        bDisplayValues[2] = 0xF;
                        bDisplayPoint = 4;
                    }
                    // hold display
                    bDisplayState = StateHold;
                    // reset pause to 3s
                    wPauseCounter = 1200;
                    // next main state
                    bMainState = StatePause;
                }
                break;
            
            case StatePause:
                // wait for start
                if(wPauseCounter == 0) {
                    // set wait period to random number
                    wWaitTime = 1200 + (wRandomNumber % 800);
                    // clear display
                    bDisplayValues[0] = 16;
                    bDisplayValues[1] = 16;
                    bDisplayValues[2] = 16;
                    bDisplayValues[3] = 16;
                    // set main state
                    bMainState = StateWaiting;
                }
                wPauseCounter--;
                break;
        }

        // display state machine
        vDisplayStateMachine();

        // check if buzzer needs to turn on
        if(wBuzzerTimer > 0) {
            pinBuzzer = 0;
            wBuzzerTimer--;
        } else {
            pinBuzzer = 1;
        }

        // increment random number
        wRandomNumber++;
        
        // wait for next update period
        while(bMainUpdate);
        bMainUpdate=1;
    }
}


void vShiftOut(uint16_t wData) {
    for(uint8_t i=0; i<16; i++) {
        // MSB first
        pinDOUT = (wData & (0x8000 >> i));
        // send clock pulse
        pinSCLK = 1;
        pinSCLK = 0;
    }
    // latch outputs
    pinLTCH = 1;
    pinLTCH = 0;
}


void vMainUpdate() {
    bMainUpdate = 0;
}


void vDisplayStateMachine() {
    // Display state machine
    switch(bDisplayState) {
        case StateOff:
            vShiftOut(0);
            break;
        
        case StateDots:
            if(bDisplayCounter > 50) {
                bDisplayCounter = 0;
                // next digit
                bDisplayPoint++;
                if(bDisplayPoint > 3) bDisplayPoint = 0;
            }
            bDisplayCounter++;
            break;
        
        case StateCount:
            if(bDisplayCounter > 4) {
                bDisplayCounter=0;
                // count up
                wDisplayTime++;
                // calculate decimal value
                bDisplayValues[0] = (wDisplayTime      ) % 10;
                bDisplayValues[1] = (wDisplayTime /  10) % 10;
                bDisplayValues[2] = (wDisplayTime / 100) % 10;
                bDisplayValues[3] = 16;
                // decimal point on 3rd digit
                bDisplayPoint = 2;
            }
            bDisplayCounter++;
            break;
        
        case StateHold:
            // hold display state
            // dont change anything in bDisplayValues and bDisplayPoint
            break;

    }

    // update display and display decimal point if necessary
    if(bDisplayPoint == bDisplayDigit)
        vShiftOut(segment_digit[bDisplayDigit] | segment_data[bDisplayValues[bDisplayDigit]] & ~(1 << 15));
    else
        vShiftOut(segment_digit[bDisplayDigit] | segment_data[bDisplayValues[bDisplayDigit]]);

    bDisplayDigit++;
    if(bDisplayDigit > 3) bDisplayDigit = 0;
}


uint8_t bCheckButton() {
    if(pinSW1 == 0) return 0;
    if(pinSW2 == 0) return 1;
    if(pinSW3 == 0) return 2;
    return 3;
}
