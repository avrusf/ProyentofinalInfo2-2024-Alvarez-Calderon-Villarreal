#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class LCD
{
private:
    LiquidCrystal_I2C lcd;

public:
    LCD(uint8_t address, int cols, int rows);
    void print(String text, int row, String alignment);
};

#endif
