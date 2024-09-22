#include "../include/KeypadInput.h"

KeypadInput::KeypadInput(char keys[4][4], byte rowPins[4], byte colPins[4], byte rows, byte cols)
    : keypad(Keypad(makeKeymap(keys), rowPins, colPins, rows, cols)) {}

String KeypadInput::read_password()
{
    String password = "";
    char key;
    while (password.length() < 6)
    {
        key = keypad.getKey();
        if (key)
        {
            if (key == '#')
            {
                break;
            }
            else if (key == '*')
            {
                if (password.length() > 0)
                {
                    password.remove(password.length() - 1);
                }
            }
            else
            {
                password += key;
            }
        }
        delay(200);
    }
    return password;
}

String KeypadInput::read_password(bool display_as_asterisks, LCD &lcd_handler)
{
    String password = "";
    char key;
    while (password.length() < 6)
    {
        key = keypad.getKey();
        if (key)
        {
            if (key == '#')
            {
                break;
            }
            else if (key == '*')
            {
                if (password.length() > 0)
                {
                    password.remove(password.length() - 1);
                    lcd_handler.print("                    ", 3, "center");
                    String asterisks(password.length(), '*');
                    lcd_handler.print(asterisks, 3, "center");
                }
            }
            else
            {
                password += key;
                if (display_as_asterisks)
                {
                    lcd_handler.print("                    ", 3, "center");
                    String asterisks(password.length(), '*');
                    lcd_handler.print(asterisks, 3, "center");
                }
                else
                {
                    lcd_handler.print(password, 3, "center");
                }
            }
        }
        delay(200);
    }
    return password;
}

char KeypadInput::read_key()
{
    char key = keypad.getKey(); // Obtener una sola tecla presionada
    return key;
}
