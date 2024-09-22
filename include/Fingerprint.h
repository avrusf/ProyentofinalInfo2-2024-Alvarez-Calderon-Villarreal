#ifndef FINGERPRINT_H
#define FINGERPRINT_H

#include "Arduino.h"
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

class Fingerprint
{
public:
    Fingerprint(HardwareSerial *serial);

    void initialize();
    bool verify_footprint();
    int save_footprint(int id);

private:
    HardwareSerial *serial;      // Guardar el puntero serial
    Adafruit_Fingerprint finger; // Inicializar el objeto sin el serial
};

#endif
