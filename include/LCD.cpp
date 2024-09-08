#include "LCD.h"

LCD::LCD(uint8_t address, int cols, int rows) : lcd(address, cols, rows)
{
    lcd.begin();
    lcd.backlight();
}

void LCD::print(String text, int row, String alignment)
{
    int col;
    if (alignment == "center")
    {
        col = (20 - text.length()) / 2;
    }
    else if (alignment == "right")
    {
        col = 20 - text.length();
    }
    else
    {
        col = 0;
    }
    lcd.setCursor(col, row);
    lcd.print(text);
}
