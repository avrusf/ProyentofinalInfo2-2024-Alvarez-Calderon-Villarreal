#ifndef KEYPAD_INPUT_H
#define KEYPAD_INPUT_H

#include <Arduino.h>
#include <Keypad.h>

class KeypadInput
{
private:
    Keypad keypad;

public:
    KeypadInput(char *keymap, byte *rowPins, byte *colPins, byte rows, byte cols);

    // Lee una tecla
    char read_key();

    // Lee una contraseña ingresada desde el teclado
    String read_password();
};

#endif
