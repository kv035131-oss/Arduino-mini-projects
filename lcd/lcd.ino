#include <LiquidCrystal_I2C.h>
#include <TimeLib.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27

void setup() {
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();

  // Set time manually: setTime(hour, minute, second, day, month, year)
  setTime(22, 20, 0, 6, 9, 2025); // 10:13 PM, 6 Sept 2025

  lcd.setCursor(0, 0);
  lcd.print("Time :");
}

void loop() {
  // Format time as HH:MM:SS
  char timeStr[9];
  sprintf(timeStr, "%02d:%02d:%02d", hour(), minute(), second());

  lcd.setCursor(0, 1);
  lcd.print(timeStr);

  delay(1000);
}
