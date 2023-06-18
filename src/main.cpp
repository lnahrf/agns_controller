#include <Arduino.h>
#include "oled.h"
#include "network.h"
#include "file_manager.h"
#include "transmitter.h"
#include "clock.h"
#include "eeprom_io.h"
#include "web.h"
#include "env.h"

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

    OLED::renderMainScreen(TRANSMITTER::printNodesRegistered(), CLOCK::getTime(), CLOCK::getDate(), WEB::getIp());
}

void loop()
{
    CLOCK::updateClient();
    OLED::renderMainScreen(TRANSMITTER::printNodesRegistered(), CLOCK::getTime(), CLOCK::getDate(), WEB::getIp());
    NETWORK::validateConnection();
    delay(3000);
}
