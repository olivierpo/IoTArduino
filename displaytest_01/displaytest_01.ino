#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Testing");

}

void loop() {
  lcd.setCursor(0, 1);
  //Print a message to second line of LCD
  lcd.print("LCD Display 1232.");
  delay(1000);

  lcd.clear();
  lcd.print("Testing");
  delay(1000);
  lcd.clear();

}
