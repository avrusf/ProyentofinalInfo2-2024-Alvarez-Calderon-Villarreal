#ifndef FINGERPRINT_H
#define FINGERPRINT_H

#include "Arduino.h"
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

class Fingerprint
{
public:
    // Cambiado de SoftwareSerial* a HardwareSerial*
    Fingerprint(HardwareSerial *serial);

    void initialize();
    bool verify_footprint();
    void save_footprint(int id);

private:
    Adafruit_Fingerprint finger;
};

#endif
