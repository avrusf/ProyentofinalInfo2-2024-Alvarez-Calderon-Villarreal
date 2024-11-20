#include "Fingerprint.h"
#include "../include/LCD.h"

LCD lcd_handler_2(0x27, 20, 4);

Fingerprint::Fingerprint(HardwareSerial *serial) : serial(serial), finger(serial) {}

void Fingerprint::initialize()
{
    finger.begin(57600);
    delay(5);
    if (finger.verifyPassword())
        Serial.println("Detectado un sensor de huella!");
    else
    {
        Serial.println("No hay comunicacion con el sensor de huella");
        Serial.println("Revise las conexiones");
        while (1)
        {
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
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK)
        return false;

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)
        return false;

    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)
        return false;

    if (finger.confidence > 100)
        return true;

    else
        return false;
}

int Fingerprint::save_footprint(int id)
{
    lcd_handler_2.initialize();
    lcd_handler_2.print("Ingrese nueva huella", 1, "center");

    int p = finger.getImage(); // LEE LA HUELLA
    while (p != FINGERPRINT_OK)
    {
        p = finger.getImage();
        switch (p)
        {
        case FINGERPRINT_OK:
            lcd_handler_2.print("Huella tomada!", 3, "center");
            Serial.println("Huella tomada");
            break;
        }
    }

    p = finger.image2Tz(1);
    switch (p)
    {
    case FINGERPRINT_OK:
        Serial.println("Huella convertida");
        break;
    case FINGERPRINT_IMAGEMESS:
        Serial.println("Huella demasiado desordenada");
        return p;
    case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Error de comunicacion");
        return p;
    case FINGERPRINT_FEATUREFAIL:
        Serial.println("No se pudieron encontrar las funciones de huellas dactilares");
        return p;
    case FINGERPRINT_INVALIDIMAGE:
        Serial.println("No se pudieron encontrar las funciones de huellas dactilares");
        return p;
    default:
        Serial.println("Error desconocido");
        return p;
    }
    lcd_handler_2.print("Retire su dedo", 3, "center");
    Serial.println("Quite el dedo");
    delay(2000); // ESPERA PARA QUE PUEDA LEERSE LA INSTRUCCION DE RETIRAR EL DEDO
    p = 0;
    while (p != FINGERPRINT_NOFINGER)
    {
        p = finger.getImage();
    }
    Serial.print("ID ");
    Serial.println(id);
    p = -1;

    lcd_handler_2.print("Coloque de nuevo", 3, "center");
    Serial.println("Coloque el mismo dedo de nuevo");
    while (p != FINGERPRINT_OK)
    {
        p = finger.getImage();
        switch (p)
        {
        case FINGERPRINT_OK:
            Serial.println("Huella tomada");
            break;
        case FINGERPRINT_NOFINGER:
            Serial.print(".");
            break;
        case FINGERPRINT_PACKETRECIEVEERR:
            Serial.println("Error de comunicacion");
            break;
        case FINGERPRINT_IMAGEFAIL:
            Serial.println("Error de huella");
            break;
        default:
            Serial.println("Error desconocido");
            break;
        }
    }

    p = finger.image2Tz(2);
    switch (p)
    {
    case FINGERPRINT_OK:
        Serial.println("Huella convertida");
        break;
    case FINGERPRINT_IMAGEMESS:
        Serial.println("Huella demasiado desordenada");
        return p;
    case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Error de comunicacion");
        return p;
    case FINGERPRINT_FEATUREFAIL:
        Serial.println("No se pudieron encontrar las funciones de huellas dactilares");
        return p;
    case FINGERPRINT_INVALIDIMAGE:
        Serial.println("No se pudieron encontrar las funciones de huellas dactilares");
        return p;
    default:
        Serial.println("Error desconocido");
        return p;
    }

    Serial.print("Creando modelo para #");
    Serial.println(id);

    p = finger.createModel();
    if (p == FINGERPRINT_OK)
    {
        Serial.println("¡Las huellas coinciden!");
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
        Serial.println("Error de comunicacion");
        return p;
    }
    else if (p == FINGERPRINT_ENROLLMISMATCH)
    {
        lcd_handler_2.print("Huellas no coinciden!", 2, "center");
        Serial.println("Las huellas dactilares no coinciden");
        return p;
    }
    else
    {
        Serial.println("Error de comunicacion");
        return p;
    }

    Serial.print("ID ");
    Serial.println(id);
    p = finger.storeModel(id);
    if (p == FINGERPRINT_OK)
    {
        lcd_handler_2.print("Huella guardada!", 3, "center");
        Serial.println("Almacenado!");
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
        Serial.println("Error de comunicacion");
        return p;
    }
    else if (p == FINGERPRINT_BADLOCATION)
    {
        Serial.println("No se pudo almacenar en esa ubicación");
        return p;
    }
    else if (p == FINGERPRINT_FLASHERR)
    {
        Serial.println("Error al escribir en flash");
        return p;
    }
    else
    {
        Serial.println("Error desconocido");
        return p;
    }
}
