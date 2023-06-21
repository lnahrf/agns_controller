#include <Arduino.h>
#include "oled.h"
#include "network.h"
#include "file_manager.h"
#include "transmitter.h"
#include "clock.h"
#include "eeprom_io.h"
#include "web.h"
#include "physical_io.h"
#include "irrigation_timer.h"
#include "env.h"

unsigned long previousTime = 0;
const int interval = 30000;

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

    PHYSICAL_IO::populateIO();
    PHYSICAL_IO::initIO(MENU_BUTTON.pin);

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

void renderHandler(const int &current)
{
    switch (current)
    {
    case 0:
        return OLED::renderMainScreen(TRANSMITTER::printNodesRegistered(),
                                      CLOCK::getTime(),
                                      CLOCK::getDate(),
                                      NETWORK::getIp());
    case 1:
        return OLED::renderScheduleScreen(EEPROM_IO::readSchedule());
    case 2:
        return OLED::renderIrrigationScreen(IRRIGATION_TIMER::getIrrigation().inProgress,
                                            IRRIGATION_TIMER::getIrrigation().durationMinutes,
                                            IRRIGATION_TIMER::getIrrigation().elapsedSeconds);
    default:
        return OLED::renderMainScreen(TRANSMITTER::printNodesRegistered(),
                                      CLOCK::getTime(),
                                      CLOCK::getDate(),
                                      NETWORK::getIp());
    }
}

void screenFunctionalityHandler(const int &current)
{
    switch (current)
    {
    case 2:
        PHYSICAL_IO::handlePotentiometerIO(MANUAL_IRRIGATION_POT.pin, [](float voltage)
                                           { IRRIGATION_TIMER::setManualIrrigationDuration(voltage); });
        break;
    default:
        break;
    }
}

void loop()
{
    int current = OLED::getCurrentScreen();

    renderHandler(current);
    screenFunctionalityHandler(current);
    intervalHandler();
    NETWORK::validateConnection();

    PHYSICAL_IO::handleButtonIO(MENU_BUTTON.pin, OLED::nextScreen, [current]()
                                {
                                    if(current != 2) return;
                                    if(IRRIGATION_TIMER::getIrrigation().inProgress)
                                        return IRRIGATION_TIMER::stopIrrigation();

                                    return IRRIGATION_TIMER::startIrrigation(); });

    IRRIGATION_TIMER::irrigationHandler();
}
