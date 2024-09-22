#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

#define SERIAL_RX_PIN 16
#define SERIAL_TX_PIN 17

// Configura un puerto serial alternativo para la comunicación con el lector de huellas
HardwareSerial mySerial(2);

// Crea un objeto Adafruit_Fingerprint con el puerto serial
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()
{
    // Inicia el puerto serial para la comunicación con el monitor serie
    Serial.begin(115200);
    // Configura el puerto serial alternativo para el lector de huellas
    mySerial.begin(57600, SERIAL_8N1, SERIAL_RX_PIN, SERIAL_TX_PIN);

    // Espera un momento para que se inicialicen las conexiones
    delay(1000);

    // Inicializa el lector de huellas con la velocidad en baudios correcta (57600 en este caso)
    finger.begin(57600);

    // Intenta obtener el modelo del sensor
    if (finger.verifyPassword())
    {
        Serial.println("Sensor de huellas verificado.");
    }
    else
    {
        Serial.println("No se pudo verificar el sensor de huellas.");
        while (1)
            ;
    }
}

void loop()
{
    Serial.println("Coloca un dedo en el sensor...");
    // Espera a que se coloque un dedo
    if (finger.getImage() == FINGERPRINT_OK)
    {
        Serial.println("Imagen de huella capturada.");
        int result = finger.image2Tz();
        if (result == FINGERPRINT_OK)
        {
            Serial.println("Huella convertida correctamente.");
        }
        else
        {
            Serial.println("Error al convertir la huella.");
        }
    }
    else
    {
        Serial.println("No se detectó ninguna huella.");
    }

    // Espera un segundo antes de intentar de nuevo
    delay(1000);
}
