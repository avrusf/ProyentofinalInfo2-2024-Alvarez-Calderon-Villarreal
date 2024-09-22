#ifndef MOVEMENT_SENSOR_H
#define MOVEMENT_SENSOR_H

class MovementSensor
{
private:
    int sensorPin;

public:
    MovementSensor(int pin);
    bool detect();
};

#endif