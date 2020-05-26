/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>

#include "HandScanner.h"

// Set GPIO_PIN if it is not defined by Arduino framework
#define PIN_BLU 2  // D4
#define PIN_RED 0 // D3
#define PIN_GRN 14 // D5
#define PIN_INP 5 // D1
// outputs D3, D4, D5
// inputs D1 (5)


HandScanner hs(PIN_RED, PIN_GRN, PIN_BLU, PIN_INP);

void setup() {
    hs.setup();
}

void loop()
{
    delay(1000);
    hs.tick();
}

