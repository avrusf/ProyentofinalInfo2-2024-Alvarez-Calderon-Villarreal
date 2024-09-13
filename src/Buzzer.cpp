#include "Buzzer.h"
#include "Pins.h"
#include <Arduino.h>

Buzzer::Buzzer(int pin) : buzzerPin(BUZZER_PIN)
{
    pinMode(buzzerPin, OUTPUT);
}

void Buzzer::setAlarm(bool state)
{
    digitalWrite(buzzerPin, state ? HIGH : LOW);
}
