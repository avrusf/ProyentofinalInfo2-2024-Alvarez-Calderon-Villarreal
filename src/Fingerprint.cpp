#include "Fingerprint.h"

Fingerprint::Fingerprint(HardwareSerial *serial) : finger(serial) {}

void Fingerprint::initialize()
{
    // SENSOR DE HUELLA
    Serial.println("Sistema de apertura con huella dactilar");
    finger.begin(57600); // inicializa comunicacion con sensor a 57600 Baudios
    delay(5);
    if (finger.verifyPassword())
        Serial.println("Detectado un sensor de huella!"); // Se muestra en el monitor serie.
    else
    { // Si no se encuentra al detector de huellas como si estuviese conectado...
        Serial.println("No hay comunicacion con el sensor de huella");
        Serial.println("Revise las conexiones");
        while (1)
        { // Se quedara en este mientras y no saldra de aqui hasta que se conecte el detector de huellas.
            delay(1);
        }
    }
    finger.getTemplateCount();
    Serial.print("El sensor contiene ");
    Serial.print(finger.templateCount);
    Serial.println(" plantillas");
    Serial.println("Esperando por una huella valida...");
}

bool Fingerprint::verify_footprint()
{
    uint8_t p = finger.getImage(); // LEE LA HUELLA
    if (p != FINGERPRINT_OK)
        return false;

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)
        return false;

    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)
        return false;

    if (finger.confidence > 100) // Si hay una huella valida
        return true;             // Devuelve Verdadero

    else
        return false; // Devuelve Falso
}

void Fingerprint::save_footprint(int id)
{
    uint8_t p = finger.getImage();
    while (p != FINGERPRINT_OK)
    {
        p = finger.getImage();
        switch (p)
        {
        case FINGERPRINT_OK:
            break;
        }
    }

    p = finger.image2Tz(1);
    delay(2000);
    p = 0;

    while (p != FINGERPRINT_NOFINGER)
    {
        p = finger.getImage();
    }

    p = -1;

    while (p != FINGERPRINT_OK)
    {
        p = finger.getImage();
        switch (p)
        {
        case FINGERPRINT_OK:
            break;
        }
    }

    p = finger.image2Tz(2);
    p = finger.createModel();
    p = finger.storeModel(id);
}