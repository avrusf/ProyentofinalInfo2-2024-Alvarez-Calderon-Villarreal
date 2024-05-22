#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 10, 8, 7, 5, 4);

void setup() {

lcd.begin(20, 4);

lcd.setCursor(0, 0);
lcd.print("Hello world!");
}

void loop() {
  // put your main code here, to run repeatedly:

}
