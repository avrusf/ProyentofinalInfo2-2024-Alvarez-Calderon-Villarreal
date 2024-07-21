#include <Arduino.h>
#include "../include/Pins.h"

#include <SoftwareSerial.h>
#include "../include/Fingerprint.h"

SoftwareSerial mySerial(SERIAL_RX_PIN, SERIAL_TX_PIN); // Crear Serial para Sensor  Rx, TX
Fingerprint myFingerprint(&mySerial);                  // Crear una instancia de la clase Fingerprint

void setup()
{
    Serial.begin(9600);
    myFingerprint.initialize();
}

void loop()
{
    if (myFingerprint.verify_footprint())
    {
        Serial.println("Huella válida detectada");
    }
    else
    {
        Serial.println("Huella no válida o no detectada");
    }
    delay(1000); // Espera 1 segundo antes de la siguiente lectura
}
