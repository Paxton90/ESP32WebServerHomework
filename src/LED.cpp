#include <Arduino.h>

int LEDPin[] = {25,17,16,27};

bool LEDState[] = {0, 0, 0, 0};

//init
void LEDInit()
{
    for (int i = 0; i < 4; i++)
    {
        pinMode(LEDPin[i], OUTPUT);
    }
}

//turn on
void LEDON(int index)
{
    digitalWrite(LEDPin[index], HIGH);
    LEDState[index] = true;
}
//turn off
void LEDOFF(int index)
{
    digitalWrite(LEDPin[index], LOW);
    LEDState[index] = false;
}
//get LED state
bool getLEDState(int index)
{
    return LEDState[index];
}