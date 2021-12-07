#include <Arduino.h>
#include "DHT.h"
#include "LED.h"

void LEDTest();
void DHTTest();

void setup()
{
    Serial.begin(115200);
    LEDInit();
}

int i = 0;

void loop()
{
    LEDTest();
}

void DHTTest()
{
    updateDHT();
    Serial.print(getTemperature());
    Serial.println(" *C");
    Serial.print(getHumidity());
    Serial.println(" H");
    delay(1000);
}

void LEDTest()
{
    LEDON(i % 4);
    (i % 4 == 0)? LEDOFF(3):LEDOFF((i % 4) - 1);
    i++;
    delay(1000);
}