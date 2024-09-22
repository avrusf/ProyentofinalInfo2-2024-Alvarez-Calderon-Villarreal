#ifndef BUZZER_H
#define BUZZER_H

class Buzzer
{
private:
    int buzzerPin;

public:
    Buzzer(int pin);
    void setAlarm(bool state);
};

#endif
