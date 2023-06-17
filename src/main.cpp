#include <Arduino.h>
#include "oled.h"
#include "transmitter.h"
#include "clock.h"
#include "eeprom_io.h"
#include "env.h"

void setup()
{
    Serial.begin(9600);

    OLED::initOLED();
    EEPROM_IO::initEEPROM();
    TRANSMITTER::initESP();
    TRANSMITTER::configAGNSNodes();
    CLOCK::initClock();
    OLED::renderMainScreen(TRANSMITTER::printNodesConnected(), CLOCK::getTime(), CLOCK::getDate());
}

void loop()
{
    CLOCK::updateClient();
    OLED::renderMainScreen(TRANSMITTER::printNodesConnected(), CLOCK::getTime(), CLOCK::getDate());
    delay(10000);
}
