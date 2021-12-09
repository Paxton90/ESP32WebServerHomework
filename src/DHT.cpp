#include <Arduino.h>
#include <SimpleDHT.h>
#define DHTPower 14
#define pinDHT11 4

SimpleDHT11 dht11(pinDHT11);

bool DHTState = false;
byte temp = 0;
byte humid = 0;

void DHTInit()
{
    pinMode(DHTPower, OUTPUT);
}

void DHTOff();

TaskHandle_t DHTTask;
// create a function for task and update data from DHT
void updateData(void *parameter)
{
    byte temperature = 0;
    byte humidity = 0;
    for (;;)
    {
        vTaskDelay(1500 / portTICK_PERIOD_MS);
        int err = SimpleDHTErrSuccess;
        if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
        {
            // Serial.print("Read DHT11 failed, err=");
            // Serial.println(err);
            continue;
        }
        temp = temperature;
        humid = humidity;
    }
}

void DHTON()
{
    if (!DHTState)
    {
        digitalWrite(DHTPower, HIGH);
        DHTState = true;
        xTaskCreate(updateData, "DHTTask", 1000, NULL, 2, &DHTTask);
    }
}

void DHTOFF()
{
    if (DHTState)
    {
        vTaskDelete(DHTTask);
        digitalWrite(DHTPower, LOW);
        DHTState = false;
    }
}

bool getDHTState()
{
    return DHTState;
}

String getTemperature()
{
    return (DHTState) ? (String)temp : "off";
}

String getHumidity()
{
    return (DHTState) ? (String)humid : "off";
}
