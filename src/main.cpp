#include <Arduino.h>
#include <string>
#include "oled.h"
#include "transmitter.h"

void setup()
{
    Serial.begin(9600);

    OLED::initOLED();

    delay(1000);

    bool esp_init = TRANSMITTER::initESP();
    if (!esp_init)
    {
        OLED::write(ESP_INIT_FAILED);
        for (;;)
            ;
    }
    OLED::write(ESP_INIT_SUCCESS);
}

void loop()
{
}
