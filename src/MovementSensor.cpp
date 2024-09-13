#include "MovementSensor.h"
#include "Pins.h"
#include <Arduino.h>

MovementSensor::MovementSensor(int pin) : sensorPin(MOVEMENT_SENSOR_PIN)
{
    pinMode(sensorPin, INPUT);
}

bool MovementSensor::detect()
{
    return digitalRead(sensorPin) == HIGH;
}
