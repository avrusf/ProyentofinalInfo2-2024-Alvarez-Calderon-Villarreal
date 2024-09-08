#include "KeypadInput.h"

KeypadInput::KeypadInput(char *keymap, byte *rowPins, byte *colPins, byte rows, byte cols)
    : keypad(makeKeymap(keymap), rowPins, colPins, rows, cols) {}

char KeypadInput::getKey()
{
    return keypad.getKey();
}
