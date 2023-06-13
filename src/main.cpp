#include <Arduino.h>
#include "oled.h"
#include "transmitter.h"
#include "clock.h"
#include "env.h"

void setup()
{
    Serial.begin(9600);

    OLED::initOLED();
    TRANSMITTER::initESP();
    TRANSMITTER::configAGNSNodes();
    CLOCK::initClock();
    OLED::mainScreen(TRANSMITTER::printNodesConnected());
}

void loop()
{
}
