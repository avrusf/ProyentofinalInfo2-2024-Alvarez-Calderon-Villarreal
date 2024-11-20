#ifndef FIREBASE_H
#define FIREBASE_H

#include <Firebase_ESP_Client.h>
#include <WiFi.h>

class FirebaseManager
{
private:
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;

public:
    FirebaseManager();
    void initialize();
    bool setBool(const char *path, bool value);
    bool getBool(const char *path, bool &value);
    bool getInt(const char *path, int &value);
    bool setInt(const char *path, int value);
};

#endif