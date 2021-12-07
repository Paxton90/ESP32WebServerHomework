#include <Arduino.h>
#include "DHT.h"
#include "LED.h"
#include "Buzzer.h"

void setup()
{
    Serial.begin(115200);
    LEDInit();
}

void loop()
{

}

/*
void DHTTest()
{
    updateDHT();
    Serial.print(getTemperature());
    Serial.println(" *C");
    Serial.print(getHumidity());
    Serial.println(" H");
    Serial.print("Core: ");
    Serial.println(xPortGetCoreID());
    delay(1000);
}

int i = 0;
void LEDTest()
{
    LEDON(i % 4);
    (i % 4 == 0) ? LEDOFF(3) : LEDOFF((i % 4) - 1);
    i++;
    Serial.print(getLEDState(3));
    Serial.print(getLEDState(2));
    Serial.print(getLEDState(1));
    Serial.println(getLEDState(0));
    Serial.print("Core: ");
    Serial.println(xPortGetCoreID());
    delay(1000);
}

void BuzzerTest(){
    Serial.println(getBuzzerState());
    buzzerAlarmOn();
    delay(1000);
    Serial.println(getBuzzerState());
    buzzerAlarmOff();
}
*/