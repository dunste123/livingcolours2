/**
 * A template for using serial stuff with arduino
 *
 * @date 2019-02-11
 * @author duncte123
 * @version 1
 */

#include <Arduino.h>

// IO
#define diPresetRgb 7
#define diPresetCmy 8
#define aoRed 9
#define aoGreen 10
#define aoBlue 11
#define aiPodMeter A0
#define doLedTest 6

#define SERIAL_RATE 115200

// Globals
bool enableDebug = true;
#define DEMO_DELAY 20

void printLn(String a_inputString) {
    if (enableDebug) {
        Serial.println(a_inputString);
    }
}

void initSerial() {
    Serial.begin(SERIAL_RATE);
    printLn("READY: Serial initialized");
}

void showCreator() {
    printLn("Created by: Duncan \"duncte123\" Sterken");
}

int getLedFromNumber(int a_colorNumber) {

    switch (a_colorNumber) {
        case 0:
            return aoRed;

        case 1:
            return aoGreen;

        case 2:
            return aoBlue;

        default:
            return aoRed;
    }

}

void initDemo() {
    printLn("INIT: Demo");

    int colourNumber = 0;
    int colourValue = 0;

    // while loop to handle 3 colours
    while (colourNumber < 3) {

        int currentLed = getLedFromNumber(colourNumber);

        printLn("Updating color to: " + (String) currentLed);

        // loop to handle one color incrementing
        while (colourValue < 256) {
            analogWrite(currentLed, colourValue);

            delay(DEMO_DELAY);

            colourValue++;
        }

        // loop to handle one color decrementing
        while (colourValue >= 0) {
            analogWrite(currentLed, colourValue);

            delay(DEMO_DELAY);

            colourValue--;
        }

        colourNumber++;
    }

    printLn("READY: Demo");
}

void setup() {

    initSerial();
    showCreator();
    initDemo();

}

void loop() {

}
