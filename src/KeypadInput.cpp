#include "KeypadInput.h"

// Constructor que inicializa el teclado matricial
KeypadInput::KeypadInput(char *keymap, byte *rowPins, byte *colPins, byte rows, byte cols)
    : keypad(makeKeymap(keymap), rowPins, colPins, rows, cols) {}

// Función que lee una tecla ingresada
char KeypadInput::read_key()
{
    char key = keypad.getKey();
    if (key)
    {
        return key;
    }
    return '\0'; // Si no hay ninguna tecla presionada, devuelve nulo
}

// Función que lee una contraseña completa del teclado
String KeypadInput::read_password()
{
    String password = "";
    char key;

    // Continúa leyendo hasta que se presione '#'
    while (true)
    {
        key = read_key();
        if (key)
        {
            if (key == '#')
            {
                break; // Salir cuando se presiona '#'
            }
            else
            {
                password += key; // Añadir tecla a la contraseña
            }
        }
        delay(100); // Pequeño retraso para evitar lectura errática
    }

    return password;
}
