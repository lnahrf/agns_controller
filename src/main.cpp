#include <Arduino.h>
#include "oled.h"
#include "transmitter.h"
#include "clock.h"
#include "eeprom_io.h"
#include "webgui.h"
#include "env.h"

unsigned long previousTime = 0;
const int interval = 10000;

void setup()
{
    Serial.begin(9600);

    OLED::initOLED();
    EEPROM_IO::initEEPROM();
    TRANSMITTER::initESP();
    TRANSMITTER::configAGNSNodes();
    CLOCK::initClock();
    WEBGUI::initServer();
    OLED::renderMainScreen(TRANSMITTER::printNodesRegistered(), CLOCK::getTime(), CLOCK::getDate(), WEBGUI::getIp());
}

void intervalHandler()
{
    unsigned long currentTime = millis();
    if (currentTime - previousTime >= interval)
    {
        previousTime = currentTime;
        CLOCK::updateClient();
        OLED::renderMainScreen(TRANSMITTER::printNodesRegistered(), CLOCK::getTime(), CLOCK::getDate(), WEBGUI::getIp());
    }
}

void loop()
{
    WEBGUI::handleRequests();
    intervalHandler();
}
