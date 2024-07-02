#include <Arduino.h>
#include "../include/constants.h"

void setup()
{
    // Inicializar el pin del LED como salida
    pinMode(LED_TEST_PIN, OUTPUT);
}

void loop()
{
    // Encender el LED
    digitalWrite(LED_TEST_PIN, HIGH);
    delay(1000); // Esperar 1 segundo

    // Apagar el LED
    digitalWrite(LED_TEST_PIN, LOW);
    delay(1000); // Esperar 1 segundo
}
