#include <Arduino.h>
#include <SimpleDHT.h>

int pinDHT11 = 4;

SimpleDHT11 dht11(pinDHT11);

byte temp = 0;
byte humid = 0;

// update data from DHT
void updateDHT()
{
    byte temperature = 0;
    byte humidity = 0;
    int err = SimpleDHTErrSuccess;
    if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
    {
        Serial.print("Read DHT11 failed, err=");
        Serial.println(err);
        delay(1000);
        return;
    }
    temp = temperature;
    humid = humid;
    //Serial.print((int)temperature); Serial.print(" *C, ");
    //Serial.print((int)humidity); Serial.println(" H");
}

int getTemperature()
{
    return temp;
}

int getHumidity()
{
    return humid;
}
