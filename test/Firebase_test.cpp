#include <Arduino.h>

#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define API_KEY ""
#define DATABASE_URL ""

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;

void setup()
{
    // Conectar a la red WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
    }
    // Asignar la API Key (requerido)
    config.api_key = API_KEY;
    // Asignar la URL de la base de datos RTDB (requerido)
    config.database_url = DATABASE_URL;
    // Registrarse en Firebase
    Firebase.signUp(&config, &auth, "", "");
    config.token_status_callback = tokenStatusCallback;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}

void loop()
{
    // Enviar datos a Firebase si esta listo, registrado y ha pasado el tiempo especificado
    if (Firebase.ready())
    {
        Firebase.RTDB.setBool(&fbdo, "datos/activacionAlarma", true);
        Firebase.RTDB.setBool(&fbdo, "datos/seguro", false);

        delay(5000);

        Firebase.RTDB.setBool(&fbdo, "datos/activacionAlarma", false);
        Firebase.RTDB.setBool(&fbdo, "datos/seguro", true);

        delay(5000);
    }
}
