#include <Arduino.h>

int sensorPir = 2;

int pirState = LOW;
int pirValue = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(sensorPir, INPUT);

    Serial.println("Esperando movimiento...");
}

void loop()
{
    pirValue = digitalRead(sensorPir);

    if (pirValue == HIGH)
    {
        if (pirState == LOW)
        {
            Serial.println("¡Movimiento detectado!");
            pirState = HIGH;
        }
    }
    else
    {
        if (pirState == HIGH)
        {
            Serial.println("Sin movimiento");
            pirState = LOW;
        }
    }
    delay(500);
}
