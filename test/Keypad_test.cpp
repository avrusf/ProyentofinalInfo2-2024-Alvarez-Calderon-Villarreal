#include <Arduino.h>
#include <Keypad.h>

// Definir el tamaño del teclado (4 filas y 4 columnas)
const byte ROWS = 4;
const byte COLS = 4;

// Definir los caracteres del teclado
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

// Pines para las filas y columnas (según los pines que proporcionaste)
byte rowPins[ROWS] = {14, 27, 26, 25}; // Conectar a los pines del ESP32
byte colPins[COLS] = {33, 32, 15, 4};  // Conectar a los pines del ESP32

// Crear la instancia del teclado
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
    // Iniciar la comunicación serial para ver el resultado en el monitor serial
    Serial.begin(115200);
}

void loop()
{
    char key = keypad.getKey(); // Leer la tecla presionada

    if (key)
    {
        Serial.println(key); // Imprimir la tecla presionada en el monitor serial
    }
}
