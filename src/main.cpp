#include <Arduino.h>
#include "../include/Pins.h"
#include <SoftwareSerial.h>
#include "../include/Fingerprint.h"
#include "../include/MovementSensor.h"
#include "../include/KeypadInput.h"
#include "../include/LCD.h"
#include "../include/Buzzer.h"

// Configuración del sensor de huellas
SoftwareSerial mySerial(SERIAL_RX_PIN, SERIAL_TX_PIN); // Crear Serial para Sensor de Huellas (Rx, TX)
Fingerprint myFingerprint(&mySerial);                  // Crear una instancia de la clase Fingerprint

// Configuración del sensor de movimiento
MovementSensor motionSensor(MOVEMENT_SENSOR_PIN);

// Configuración del teclado matricial
KeypadInput keypadHandler;

// Configuración del LCD
LCD lcdHandler;

// Configuración del zumbador
Buzzer buzzer(BUZZER_PIN);

// Clave de ejemplo
String correctPassword = "1234";

// Variables de estado
bool surveillanceMode = false;

void activateSurveillance()
{
    lcdHandler.print("Activando vigilancia", 0, "center");

    // Verificar huella
    lcdHandler.print("Verificar huella", 1, "center");
    if (myFingerprint.verify_footprint())
    {
        lcdHandler.print("Huella válida", 2, "center");
        Serial.println("Huella válida detectada");

        // Ingresar clave
        lcdHandler.print("Ingrese clave:", 3, "center");
        String enteredPassword = keypadHandler.readPassword();

        if (enteredPassword == correctPassword)
        {
            surveillanceMode = true;
            lcdHandler.print("Vigilancia Activada", 2, "center");
            Serial.println("Vigilancia Activada");
            delay(2000);
        }
        else
        {
            lcdHandler.print("Clave Incorrecta!", 2, "center");
            Serial.println("Clave Incorrecta!");
            buzzer.setAlarm(true);  // Activar alarma
            delay(5000);            // Mantener la alarma activa por 5 segundos
            buzzer.setAlarm(false); // Desactivar alarma
        }
    }
    else
    {
        lcdHandler.print("Huella no válida!", 2, "center");
        Serial.println("Huella no válida o no detectada");
        buzzer.setAlarm(true);  // Activar alarma
        delay(5000);            // Mantener la alarma activa por 5 segundos
        buzzer.setAlarm(false); // Desactivar alarma
    }
}

void changePassword()
{
    lcdHandler.print("Cambio de Clave", 0, "center");

    // Verificar clave actual
    lcdHandler.print("Ingrese clave actual:", 1, "center");
    String enteredPassword = keypadHandler.readPassword();

    if (enteredPassword == correctPassword)
    {
        lcdHandler.print("Ingrese nueva clave:", 2, "center");
        String newPassword = keypadHandler.readPassword();

        lcdHandler.print("Repita nueva clave:", 3, "center");
        String confirmPassword = keypadHandler.readPassword();

        if (newPassword == confirmPassword)
        {
            correctPassword = newPassword;
            lcdHandler.print("Clave cambiada!", 2, "center");
            Serial.println("Clave cambiada exitosamente");
        }
        else
        {
            lcdHandler.print("Claves no coinciden", 2, "center");
            Serial.println("Las claves no coinciden");
        }
    }
    else
    {
        lcdHandler.print("Clave Incorrecta!", 2, "center");
        Serial.println("Clave Incorrecta!");
    }

    delay(2000);
}

void addFingerprint()
{
    lcdHandler.print("Agregar Huella", 0, "center");

    // Verificar huella actual
    lcdHandler.print("Verificar huella", 1, "center");
    if (myFingerprint.verify_footprint())
    {
        lcdHandler.print("Huella válida", 2, "center");
        Serial.println("Huella válida detectada");

        lcdHandler.print("Ingrese nueva huella", 3, "center");
        // Aquí iría el código para agregar la nueva huella
        // Por ejemplo: myFingerprint.enrollFingerprint();
    }
    else
    {
        lcdHandler.print("Huella no válida!", 2, "center");
        Serial.println("Huella no válida o no detectada");
    }

    delay(2000);
}

void displayMenu()
{
    lcdHandler.print("1) Activar vigilancia", 0, "left");
    lcdHandler.print("2) Cambiar clave", 1, "left");
    lcdHandler.print("3) Agregar huella", 2, "left");
    lcdHandler.print("Seleccione una opcion:", 3, "left");

    char option = keypadHandler.readKey();

    switch (option)
    {
    case '1':
        activateSurveillance();
        break;
    case '2':
        changePassword();
        break;
    case '3':
        addFingerprint();
        break;
    default:
        lcdHandler.print("Opción inválida", 2, "center");
        delay(2000);
        break;
    }
}

void loop()
{
    if (surveillanceMode)
    {
        if (motionSensor.detect())
        {
            lcdHandler.print("Movimiento detectado!", 0, "center");
            Serial.println("Movimiento detectado!");

            // Verificar huella
            lcdHandler.print("Verificar huella", 1, "center");
            if (myFingerprint.verify_footprint())
            {
                lcdHandler.print("Huella válida", 2, "center");
                Serial.println("Huella válida detectada");

                // Ingresar clave
                lcdHandler.print("Ingrese clave:", 3, "center");
                String enteredPassword = keypadHandler.readPassword();

                if (enteredPassword == correctPassword)
                {
                    lcdHandler.print("Acceso Permitido", 2, "center");
                    Serial.println("Acceso Permitido");
                    delay(2000);
                }
                else
                {
                    lcdHandler.print("Clave Incorrecta!", 2, "center");
                    Serial.println("Clave Incorrecta!");
                    buzzer.setAlarm(true);  // Activar alarma
                    delay(5000);            // Mantener la alarma activa por 5 segundos
                    buzzer.setAlarm(false); // Desactivar alarma
                }
            }
            else
            {
                lcdHandler.print("Huella no válida!", 2, "center");
                Serial.println("Huella no válida o no detectada");
                buzzer.setAlarm(true);  // Activar alarma
                delay(5000);            // Mantener la alarma activa por 5 segundos
                buzzer.setAlarm(false); // Desactivar alarma
            }
        }
        delay(1000); // Espera 1 segundo antes de la siguiente iteración
    }
    else
    {
        displayMenu();
    }
}
