#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Dirección del módulo I2C. Cambiar si es diferente (0x27 o 0x3F son comunes).
LiquidCrystal_I2C lcd(0x27, 20, 4); // LCD de 20 columnas y 4 filas

void setup()
{
    // Inicia la comunicación I2C
    Wire.begin(21, 22); // SDA en pin 21, SCL en pin 22

    // Inicializa la pantalla LCD
    lcd.init();
    lcd.backlight(); // Activa la luz de fondo

    // Test de escritura
    lcd.setCursor(0, 0); // Primer fila, primera columna
    lcd.print("Test de LCD 20x4");

    lcd.setCursor(0, 1); // Segunda fila, primera columna
    lcd.print("Line 1");

    lcd.setCursor(0, 2); // Tercera fila, primera columna
    lcd.print("Line 2");

    lcd.setCursor(0, 3); // Cuarta fila, primera columna
    lcd.print("ESP32 I2C Test");
}

void loop()
{
    // No se necesita hacer nada en el loop para este test
}
