#include <Arduino.h>
#include "oled.h"
#include "network.h"
#include "file_manager.h"
#include "transmitter.h"
#include "clock.h"
#include "eeprom_io.h"
#include "web.h"
#include "env.h"

unsigned long previousTime = 0;
const int interval = 10000;

void setup()
{
    Serial.begin(9600);

    OLED::initOLED();
    FILE_MANAGER::initSPIFFS();
    NETWORK::connect();
    WEB::initServer();
    CLOCK::initClock();
    EEPROM_IO::initEEPROM();
    TRANSMITTER::initESP();
    TRANSMITTER::configAGNSNodes();

    OLED::renderMainScreen(TRANSMITTER::printNodesRegistered(), CLOCK::getTime(), CLOCK::getDate(), NETWORK::getIp());
}

void intervalHandler()
{
    unsigned long currentTime = millis();

    if (currentTime - previousTime >= interval)
    {
        CLOCK::updateClient();
        previousTime = currentTime;
    }
}

void loop()
{
    intervalHandler();
    NETWORK::validateConnection();
    OLED::renderMainScreen(TRANSMITTER::printNodesRegistered(), CLOCK::getTime(), CLOCK::getDate(), NETWORK::getIp());
    delay(3000);
}
