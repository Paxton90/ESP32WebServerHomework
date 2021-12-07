#include <ESP32Servo.h>
#include "Buzzer.h"

int buzzer = 26;
bool buzzerState = false;

TaskHandle_t AlarmTask;

// function for task
void buzzerAlarm(void *parameter)
{
    buzzerState = true;
    for (;;)
    {
        tone(buzzer, 1000, 250);
        // Serial.print("Core: ");
        // Serial.println(xPortGetCoreID());
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

// create task to run in core 0, default loop by arduino is core 1
void buzzerAlarmOn()
{
    xTaskCreatePinnedToCore(buzzerAlarm, "AlarmTask", 1000, NULL, 0, &AlarmTask, 0);
}
// delete task to stop
void buzzerAlarmOff()
{
    vTaskDelete(AlarmTask);
    buzzerState = false;
}

bool getBuzzerState()
{
    return buzzerState;
}