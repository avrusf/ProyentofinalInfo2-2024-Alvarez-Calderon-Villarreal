#include "Firebase.h"
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#define WIFI_SSID "iPhone de Santino"
#define WIFI_PASSWORD "Santienzo04"

#define API_KEY "AIzaSyBp955Z4IdNCtkYxWCqrMyzv18I9sL9sxU"
#define DATABASE_URL "https://poryectofinal-8a52c-default-rtdb.firebaseio.com/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;

// Constructor
FirebaseManager::FirebaseManager()
{
    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;

    // Callback para token
    config.token_status_callback = tokenStatusCallback;

    // Autenticación anónima
    auth.user.email = "";
    auth.user.password = "";

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}

void FirebaseManager::initialize()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Conectando a WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWiFi conectado.");
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

bool FirebaseManager::setBool(const char *path, bool value)
{
    Serial.print("Heap disponible: ");
    Serial.println(ESP.getFreeHeap());

    if (Firebase.RTDB.setBool(&fbdo, path, value))
    {
        Serial.println("Dato booleano escrito correctamente en Firebase.");
        return true;
    }
    else
    {
        Serial.printf("Error escribiendo dato booleano: %s\n", fbdo.errorReason().c_str());
        return false;
    }
}

bool FirebaseManager::getBool(const char *path, bool &value)
{
    Serial.print("Heap disponible: ");
    Serial.println(ESP.getFreeHeap());

    if (Firebase.RTDB.getBool(&fbdo, path))
    {
        value = fbdo.boolData();
        Serial.println("Dato booleano leído correctamente desde Firebase.");
        return true;
    }
    else
    {
        Serial.printf("Error leyendo dato booleano: %s\n", fbdo.errorReason().c_str());
        return false;
    }
}

bool FirebaseManager::getInt(const char *path, int &value)
{
    Serial.print("Heap disponible: ");
    Serial.println(ESP.getFreeHeap());

    if (Firebase.RTDB.getInt(&fbdo, path))
    {
        value = fbdo.intData();
        Serial.println("Dato entero leído correctamente desde Firebase.");
        return true;
    }
    else
    {
        Serial.printf("Error leyendo dato entero: %s\n", fbdo.errorReason().c_str());
        return false;
    }
}

bool FirebaseManager::setInt(const char *path, int value)
{
    Serial.print("Heap disponible: ");
    Serial.println(ESP.getFreeHeap());

    if (Firebase.RTDB.setInt(&fbdo, path, value))
    {
        Serial.println("Dato entero escrito correctamente en Firebase.");
        return true;
    }
    else
    {
        Serial.printf("Error escribiendo dato entero: %s\n", fbdo.errorReason().c_str());
        return false;
    }
}