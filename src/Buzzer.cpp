#include <ESP32Servo.h>

int buzzer = 26;
bool buzzerState = false;

TaskHandle_t AlarmTask;

// function for task
void buzzerAlarm(void *parameter)
{
    for (;;)
    {
        tone(buzzer, 1000, 250);
        // Serial.print("Core: ");
        // Serial.println(xPortGetCoreID());
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

// create task to run in free core
void buzzerAlarmON()
{
    if (!buzzerState)
    {
        buzzerState = true;
        xTaskCreate(buzzerAlarm, "AlarmTask", 1000, NULL, 0, &AlarmTask);
    }
}
// delete task and stop buzzer
void buzzerAlarmOFF()
{
    if (buzzerState)
    {
        vTaskDelete(AlarmTask);
        tone(buzzer, 0, 0);
        buzzerState = false;
    }
}

bool getBuzzerState()
{
    return buzzerState;
}