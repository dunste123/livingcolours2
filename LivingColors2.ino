/**
 * A template for using serial stuff with arduino
 *
 * Remote library: https://github.com/z3t0/Arduino-IRremote
 *
 * @date 2019-02-11
 * @author duncte123
 * @version 1
 */

#include <Arduino.h>

// The IR libs
#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

// IO
#define diPresetRgb 0xFFC23D
#define diPresetCmy 0xFF906F
#define diStepButton 0xFF629D
#define diPlusButton 0xFFE21D
#define diMinButton 0xFFA25D
#define aoRed 9
#define aoGreen 10
#define aoBlue 5
//#define aiPodMeter A0
#define diIrReceiver 2 // Pin 1 on the receiver
// pin 2 on the receiver is GND
// pin 3 on the receiver is Vcc

// led values
int selectedLed = 0;

int ledValues[] = {0, 0, 0};

//IR
IRrecv irrecv(diIrReceiver);           // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'

#define SERIAL_RATE 115200

// Globals
#define ENABLE_DEBUG true
bool rgbMode = false;
#define DEMO_DELAY 10

void printLn(String a_inputString) {
#if ENABLE_DEBUG
    Serial.println(a_inputString);
#endif
}

void initSerial() {
    Serial.begin(SERIAL_RATE);
    printLn("READY: Serial initialized");
}

void initPins() {
    printLn("INIT: IR");

    pinMode(diIrReceiver, INPUT);
    irrecv.enableIRIn();

    printLn("READY: IR");
}

void showCreator() {
    printLn("Created by: Duncan \"duncte123\" Sterken");
}

int getLedFromNumber(int a_colorNumber) {

    switch (a_colorNumber) {
        case 0:
        case aoRed:
            return aoRed;

        case 1:
        case aoGreen:
            return aoGreen;

        case 2:
        case aoBlue:
            return aoBlue;

        default:
            return aoRed;
    }

}

void writeLedColors() {
    for (int i = 0; i < 3; i++) {
        printLn("Writing led: " + (String) i);

        int led = getLedFromNumber(i);
        int value = ledValues[i];

        if (value >= 255) {
            value = 255;
        }

        if (value < 0) {
            value = 0;
        }

        ledValues[i] = value;

        analogWrite(led, value);

        delay(10);
    }
}

void setLedColor(int led, int value) {

    if (value >= 255) {
        value = 255;
    }

    if (value < 0) {
        value = 0;
    }

    ledValues[led] = value;

    int ledPin = getLedFromNumber(led);

    analogWrite(ledPin, value);
}

int getLedColor(int led) {
    return ledValues[led];
}

void initDemo() {
    printLn("INIT: Demo");

    int colourNumber = 0;
    int colourValue = 0;

    // while loop to handle 3 colours
    while (colourNumber < 3) {
        printLn("Updating color to: " + (String) colourNumber);

        // loop to handle one color incrementing
        while (colourValue < 256) {
            setLedColor(colourNumber, colourValue);

            delay(DEMO_DELAY);

            colourValue++;
        }

        // loop to handle one color decrementing
        while (colourValue >= 0) {
            setLedColor(colourNumber, colourValue);

            delay(DEMO_DELAY);

            colourValue--;
        }

        colourNumber++;
        colourValue = 0;
    }

    printLn("READY: Demo");
}

void setup() {

    initSerial();
    initPins();
    showCreator();
    initDemo();

}

void handleCmy() {

    static int m_colorCmyNr = 0;

    printLn("Cmy mode: " + (String) m_colorCmyNr);

    switch (m_colorCmyNr) {

        case 0:
            ledValues[0] = 0;
            ledValues[1] = 255;
            ledValues[2] = 255;

            printLn("Cyan");
            break;

        case 1:
            ledValues[0] = 255;
            ledValues[1] = 0;
            ledValues[2] = 255;

            printLn("Magenta");
            break;

        default:
            ledValues[0] = 255;
            ledValues[1] = 255;
            ledValues[2] = 0;

            printLn("Yellow");
            break;
    }

    writeLedColors();

    m_colorCmyNr++;

    if (m_colorCmyNr > 2) {
        m_colorCmyNr = 0;
    }
}

void handleRgb() {

    // select led
    // dim led

    selectedLed++;

    printLn("RGB mode: " + (String) selectedLed);

    if (selectedLed > 3) {
        selectedLed = 0;
    }
}

void incrementLedValue() {
    if (selectedLed > 2 || !rgbMode) {
        return;
    }

    int ledValue = getLedColor(selectedLed);

    ledValue = ledValue + 10;

    if (ledValue >= 255) {
        ledValue = 255;
    }

    printLn((String) "Incrementing " + selectedLed + " new value " + ledValue);

    setLedColor(selectedLed, ledValue);
}

void decrementLedValue() {
    if (selectedLed > 2 || !rgbMode) {
        return;
    }

    int ledValue = getLedColor(selectedLed);

    ledValue = ledValue - 10;

    if (ledValue < 0) {
        ledValue = 0;
    }

    printLn((String) "Decrementing " + selectedLed + " new value " + ledValue);

    setLedColor(selectedLed, ledValue);
}

void handleStepButton() {
    if (rgbMode) {
        handleRgb();
    } else {
        handleCmy();
    }
}

void handlePresets() {
    if (irrecv.decode(&results)) {
//        Serial.println(results.value, HEX);  // UN Comment to see raw values

        if (results.value == diPresetCmy) {
            rgbMode = false;
            printLn("MODE SET: cmy");
        }

        if (results.value == diPresetRgb) {
            rgbMode = true;
            printLn("MODE SET: rgb");
        }

        if (results.value == diStepButton) {
            handleStepButton();
        }

        if (results.value == diPlusButton) {
            incrementLedValue();
        }

        if (results.value == diMinButton) {
            decrementLedValue();
        }

        delay(500);

        irrecv.resume(); // receive the next value
    }
}

void loop() {
    handlePresets();
}
