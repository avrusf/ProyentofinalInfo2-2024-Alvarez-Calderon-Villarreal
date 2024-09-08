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
    char getKey();
};

#endif
