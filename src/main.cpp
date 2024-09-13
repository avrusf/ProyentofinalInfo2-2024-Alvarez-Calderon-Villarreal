#include <Arduino.h>
#include "../include/Pins.h"
#include "../include/Fingerprint.h"
#include "../include/MovementSensor.h"
#include "../include/KeypadInput.h"
#include "../include/LCD.h"
#include "../include/Buzzer.h"

const byte ROWS = 4;
const byte COLS = 4;
char keys[] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

// Configuración del sensor de huellas
HardwareSerial my_serial(2);            // Usar Serial1 (puede ser 0, 1, o 2)
Fingerprint my_fingerprint(&my_serial); // Crear una instancia de la clase Fingerprint

// Configuración del sensor de movimiento
MovementSensor motion_sensor(MOVEMENT_SENSOR_PIN);

// Configuración del teclado matricial
KeypadInput keypad_handler(keys, rowPins, colPins, ROWS, COLS);

// Configuración del LCD
LCD lcd_handler(0x27, 20, 4);

// Configuración del zumbador
Buzzer buzzer(BUZZER_PIN);

// Clave de ejemplo
String correct_password = "1234";

// Variables de estado
bool surveillance_mode = false;

void setup()
{
    // Inicializar Serial1 con la velocidad en baudios deseada
    my_serial.begin(57600, SERIAL_8N1, SERIAL_RX_PIN, SERIAL_TX_PIN);
    Serial.begin(115200); // Para depuración
}

void activate_surveillance()
{
    lcd_handler.print("Activando vigilancia", 0, "center");

    // Verificar huella
    lcd_handler.print("Verificar huella", 1, "center");
    if (my_fingerprint.verify_footprint())
    {
        lcd_handler.print("Huella válida", 2, "center");
        Serial.println("Huella válida detectada");

        // Ingresar clave
        lcd_handler.print("Ingrese clave:", 3, "center");
        String entered_password = keypad_handler.read_password();

        if (entered_password == correct_password)
        {
            surveillance_mode = true;
            lcd_handler.print("Vigilancia Activada", 2, "center");
            Serial.println("Vigilancia Activada");
            delay(2000);
        }
        else
        {
            lcd_handler.print("Clave Incorrecta!", 2, "center");
            Serial.println("Clave Incorrecta!");
            buzzer.setAlarm(true);  // Activar alarma
            delay(5000);            // Mantener la alarma activa por 5 segundos
            buzzer.setAlarm(false); // Desactivar alarma
        }
    }
    else
    {
        lcd_handler.print("Huella no válida!", 2, "center");
        Serial.println("Huella no válida o no detectada");
        buzzer.setAlarm(true);  // Activar alarma
        delay(5000);            // Mantener la alarma activa por 5 segundos
        buzzer.setAlarm(false); // Desactivar alarma
    }
}

void change_password()
{
    lcd_handler.print("Cambio de Clave", 0, "center");

    // Verificar clave actual
    lcd_handler.print("Ingrese clave actual:", 1, "center");
    String entered_password = keypad_handler.read_password();

    if (entered_password == correct_password)
    {
        lcd_handler.print("Ingrese nueva clave:", 2, "center");
        String new_password = keypad_handler.read_password();

        lcd_handler.print("Repita nueva clave:", 3, "center");
        String confirm_password = keypad_handler.read_password();

        if (new_password == confirm_password)
        {
            correct_password = new_password;
            lcd_handler.print("Clave cambiada!", 2, "center");
            Serial.println("Clave cambiada exitosamente");
        }
        else
        {
            lcd_handler.print("Claves no coinciden", 2, "center");
            Serial.println("Las claves no coinciden");
        }
    }
    else
    {
        lcd_handler.print("Clave Incorrecta!", 2, "center");
        Serial.println("Clave Incorrecta!");
    }

    delay(2000);
}

void add_fingerprint()
{
    lcd_handler.print("Agregar Huella", 0, "center");

    // Verificar huella actual
    lcd_handler.print("Verificar huella", 1, "center");
    if (my_fingerprint.verify_footprint())
    {
        lcd_handler.print("Huella válida", 2, "center");
        Serial.println("Huella válida detectada");

        lcd_handler.print("Ingrese nueva huella", 3, "center");
        // Aquí iría el código para agregar la nueva huella
        my_fingerprint.save_footprint(10); // Ejemplo
    }
    else
    {
        lcd_handler.print("Huella no válida!", 2, "center");
        Serial.println("Huella no válida o no detectada");
    }

    delay(2000);
}

void display_menu()
{
    lcd_handler.print("1) Activar vigilancia", 0, "left");
    lcd_handler.print("2) Cambiar clave", 1, "left");
    lcd_handler.print("3) Agregar huella", 2, "left");
    lcd_handler.print("Seleccione una opcion:", 3, "left");

    char option = keypad_handler.read_key();

    switch (option)
    {
    case '1':
        activate_surveillance();
        break;
    case '2':
        change_password();
        break;
    case '3':
        add_fingerprint();
        break;
    default:
        lcd_handler.print("Opción inválida", 2, "center");
        delay(2000);
        break;
    }
}

void loop()
{
    if (surveillance_mode)
    {
        if (motion_sensor.detect())
        {
            lcd_handler.print("Movimiento detectado!", 0, "center");
            Serial.println("Movimiento detectado!");

            // Verificar huella
            lcd_handler.print("Verificar huella", 1, "center");
            if (my_fingerprint.verify_footprint())
            {
                lcd_handler.print("Huella válida", 2, "center");
                Serial.println("Huella válida detectada");

                // Ingresar clave
                lcd_handler.print("Ingrese clave:", 3, "center");
                String entered_password = keypad_handler.read_password();

                if (entered_password == correct_password)
                {
                    lcd_handler.print("Acceso Permitido", 2, "center");
                    Serial.println("Acceso Permitido");
                    delay(2000);
                }
                else
                {
                    lcd_handler.print("Clave Incorrecta!", 2, "center");
                    Serial.println("Clave Incorrecta!");
                    buzzer.setAlarm(true);  // Activar alarma
                    delay(5000);            // Mantener la alarma activa por 5 segundos
                    buzzer.setAlarm(false); // Desactivar alarma
                }
            }
            else
            {
                lcd_handler.print("Huella no válida!", 2, "center");
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
        display_menu();
    }
}
