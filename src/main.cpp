#include <Arduino.h>
#include "../include/Pins.h"
#include "../include/Fingerprint.h"
#include "../include/MovementSensor.h"
#include "../include/KeypadInput.h"
#include "../include/LCD.h"
#include "../include/Buzzer.h"

#include <EEPROM.h>

#define EEPROM_SIZE 4    // Espacio necesario para almacenar un entero (4 bytes)
#define EEPROM_ADDRESS 0 // Dirección donde se guardará el entero en la EEPROM

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {14, 27, 26, 25}; // Pines del ESP32
byte colPins[COLS] = {33, 32, 15, 4};  // Pines del ESP32

// Componentes conectados
HardwareSerial my_serial(2);
Fingerprint my_fingerprint(&my_serial);
MovementSensor motion_sensor(MOVEMENT_SENSOR_PIN);
KeypadInput keypad_handler(keys, rowPins, colPins, ROWS, COLS);
LCD lcd_handler(0x27, 20, 4);
Buzzer buzzer(BUZZER_PIN);

// Clave de acceso
String correct_password = "1234";

const unsigned long TIME_LIMIT = 10000; // Límite de tiempo en milisegundos (10 segundos)

// Variables de estado
bool surveillance_mode = false;

void setup()
{
    my_serial.begin(57600, SERIAL_8N1, SERIAL_RX_PIN, SERIAL_TX_PIN);
    Serial.begin(115200);
    lcd_handler.initialize();
    my_fingerprint.initialize();
    Serial.println("Sistema iniciado");

    if (!EEPROM.begin(EEPROM_SIZE))
    {
        Serial.println("Error inicializando EEPROM");
        return;
    }
}

int increment_and_store_number()
{
    // Leer el número almacenado en la EEPROM
    int stored_number = 5;
    EEPROM.get(EEPROM_ADDRESS, stored_number);

    // Incrementar el número
    stored_number += 1;

    // Guardar el nuevo número en la EEPROM
    EEPROM.put(EEPROM_ADDRESS, stored_number);

    // Asegurarse de que los cambios se escriben en la EEPROM
    EEPROM.commit();

    // Retornar el número incrementado
    return stored_number;
}

// Verificación de huella con límite de tiempo
bool verify_fingerprint(unsigned long time_limit)
{
    unsigned long start_time = millis(); // Tiempo de inicio
    bool valid_fingerprint = false;

    while (millis() - start_time < time_limit) // Mientras no se haya superado el límite de tiempo
    {
        if (my_fingerprint.verify_footprint())
        {
            lcd_handler.clean();
            lcd_handler.print("Verificar huella", 0, "center");
            lcd_handler.print("Huella valida", 2, "center"); // Mostrar el resultado en la tercera línea
            Serial.println("Huella valida");
            valid_fingerprint = true;
            delay(5000);
            lcd_handler.clean();
            return true; // Retorna verdadero si la huella es valida
        }
        else
        {
            lcd_handler.clean();
            lcd_handler.print("Verificar huella", 0, "center");
            lcd_handler.print("Huella no valida", 2, "center"); // Mostrar el resultado en la tercera línea
            Serial.println("Huella no valida");
            delay(1000); // Pausa de 1 segundo antes de intentar de nuevo
        }

        // Si el tiempo límite ha sido superado
        if (millis() - start_time >= time_limit)
        {
            lcd_handler.clean();
            lcd_handler.print("Verificar huella", 0, "center");
            lcd_handler.print("Tiempo agotado!", 2, "center"); // Mostrar mensaje de tiempo agotado
            Serial.println("Tiempo agotado para verificar huella");
            delay(5000);
            lcd_handler.clean();
            return false; // Retorna falso si el tiempo se ha agotado
        }
    }

    delay(5000);
    lcd_handler.clean();
    return false; // Retorna falso si no se detectó una huella valida dentro del tiempo límite
}

// Verificación de clave
bool verify_password(unsigned long time_limit)
{
    unsigned long start_time = millis(); // Tiempo de inicio
    bool correct_password_entered = false;

    lcd_handler.clean();
    lcd_handler.print("Ingrese clave", 0, "center"); // Mostrar mensaje en la primera línea

    while (millis() - start_time < time_limit)
    {
        String entered_password = keypad_handler.read_password();
        if (entered_password == correct_password)
        {
            lcd_handler.print("Clave correcta", 2, "center"); // Mostrar resultado en la tercera línea
            Serial.println("Clave correcta");
            delay(5000);
            return true;
        }
        else
        {
            lcd_handler.print("Clave incorrecta", 2, "center"); // Mostrar error en la tercera línea
            Serial.println("Clave incorrecta");
            buzzer.setAlarm(true);
            delay(5000);
            buzzer.setAlarm(false);
            return false;
        }
        // Si el tiempo límite ha sido superado
        if (millis() - start_time >= time_limit)
        {
            lcd_handler.clean();
            lcd_handler.print("Verificar clave", 0, "center");
            lcd_handler.print("Tiempo agotado!", 2, "center"); // Mostrar mensaje de tiempo agotado
            Serial.println("Tiempo agotado para verificar clave");
            delay(5000);
            lcd_handler.clean();
            return false; // Retorna falso si el tiempo se ha agotado
        }
    }
    delay(5000);
    lcd_handler.clean();
    return false; // Retorna falso si no se detectó una clave valida dentro del tiempo límite
}

// Cambio de clave
void change_password()
{
    lcd_handler.clean();
    lcd_handler.print("Cambio de Clave", 0, "center");

    if (verify_password(TIME_LIMIT))
    {
        lcd_handler.print("Nueva clave:", 0, "center");
        String new_password = keypad_handler.read_password(true, lcd_handler); // Pasamos lcd_handler
        lcd_handler.print("Confirme clave:", 0, "center");
        String confirm_password = keypad_handler.read_password(true, lcd_handler); // Pasamos lcd_handler

        if (new_password == confirm_password)
        {
            correct_password = new_password;
            lcd_handler.print("Clave cambiada", 2, "center");
            Serial.println("Clave cambiada");
        }
        else
        {
            lcd_handler.print("Claves no coinciden", 2, "center");
            Serial.println("Las claves no coinciden");
        }
    }
    delay(2000);
    lcd_handler.clean();
    return;
}

// Agregar huella
void add_fingerprint()
{
    lcd_handler.clean();
    lcd_handler.print("Agregar Huella", 0, "center"); // Mostrar título en la primera línea
    if (verify_fingerprint(TIME_LIMIT))
    {
        lcd_handler.print("Ingrese nueva huella", 1, "center");      // Mostrar solicitud de huella en la cuarta línea
        my_fingerprint.save_footprint(increment_and_store_number()); // Ejemplo
    }
    delay(2000);
}

// Activar vigilancia
void activate_surveillance()
{
    if (verify_fingerprint(TIME_LIMIT) && verify_password(TIME_LIMIT))
    {
        surveillance_mode = true;
        // lcd_handler.print("Vigilancia activada ", 1, "center"); // Mostrar estado activado en la tercera línea
        Serial.println("Vigilancia activada");
        delay(2000);
    }
}

// Menú principal
void display_menu()
{
    lcd_handler.print("1) Vigilancia activa", 0, "left"); // Mostrar opciones del menú en la primera línea
    lcd_handler.print("2) Cambiar clave    ", 1, "left"); // Mostrar segunda opción en la segunda línea
    lcd_handler.print("3) Agregar huella   ", 2, "left"); // Mostrar tercera opción en la tercera línea
    lcd_handler.print("Seleccione opcion   ", 3, "left"); // Solicitar opción en la cuarta línea

    char option = keypad_handler.read_key();
    switch (option)
    {
    case '1':
        lcd_handler.clean();
        activate_surveillance();
        break;
    case '2':
        lcd_handler.clean();
        change_password();
        break;
    case '3':
        lcd_handler.clean();
        add_fingerprint();
        break;
    default:
        break;
    }
}

void monitor_surveillance()
{
    if (motion_sensor.detect())
    {
        unsigned long start_time = millis();                    // Guardar el tiempo de inicio
        lcd_handler.print("Movimiento detectado", 0, "center"); // Mostrar aviso de detección de movimiento en la primera línea
        Serial.println("Movimiento detectado");

        bool valid_fingerprint = false;
        bool correct_password_entered = false;

        while (millis() - start_time < TIME_LIMIT) // Mientras no se haya superado el límite de tiempo
        {
            valid_fingerprint = verify_fingerprint(TIME_LIMIT);

            if (valid_fingerprint)
            {
                lcd_handler.print("Ingrese clave:", 1, "center"); // Solicitar clave en la segunda línea
                correct_password_entered = verify_password(TIME_LIMIT);

                if (correct_password_entered)
                {
                    surveillance_mode = false;
                    lcd_handler.print("Vigilancia desactivada", 2, "center"); // Mostrar éxito en la tercera línea
                    Serial.println("Vigilancia desactivada");
                    delay(2000); // Dar tiempo para mostrar el mensaje
                    return;      // Salir de la función después de desactivar la vigilancia
                }
            }
            else
            {
                lcd_handler.print("Huella no valida", 2, "center"); // Mostrar error en la tercera línea
            }

            delay(1000); // Esperar 1 segundo antes de volver a intentar
        }

        // Si se superó el tiempo límite
        lcd_handler.print("Tiempo agotado!", 2, "center"); // Mostrar error en la tercera línea
        Serial.println("Tiempo agotado! Activando alarma");
        buzzer.setAlarm(true);  // Activar alarma
        delay(5000);            // Mantener la alarma activa por 5 segundos
        buzzer.setAlarm(false); // Desactivar alarma
    }
}

void loop()
{
    if (surveillance_mode)
    {
        monitor_surveillance();
    }
    else
    {
        display_menu();
    }
}
