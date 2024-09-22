#ifndef KEYPADINPUT_H
#define KEYPADINPUT_H

#include <Keypad.h>
#include "LCD.h"

class KeypadInput
{
private:
    Keypad keypad;

public:
    KeypadInput(char keys[4][4], byte rowPins[4], byte colPins[4], byte rows, byte cols);

    String read_password();
    String read_password(bool display_as_asterisks, LCD &lcd_handler); // Cambiamos la firma para incluir lcd_handler
    char read_key();
};

#endif
