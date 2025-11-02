#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>
#include <time.h>

// RC522 setup
#define SS_PIN 5
#define RST_PIN 22
MFRC522 rfid(SS_PIN, RST_PIN);

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2); // Use 0x3F if 0x27 doesn't work

// Wi-Fi credentials
const char* ssid = "Shrinidhi";
const char* password = "11111112";

// Google Sheets Web App URL
const char* scriptURL = "https://script.google.com/macros/s/AKfycbxxCiUepEES_f5KgZ38Q0sjtW6Yr4Y6E85kGb81CUt4zaHUOqpEu5hh5glsUIJoXL5s/exec";

// UID-to-name mapping
String getNameFromUID(String uid) {
  uid.toLowerCase(); // Normalize UID
  if (uid == "43328604") return "K VIJAY KUMAR";
  return "UNKNOWN";
}

void setup() {
  Serial.begin(115200);

  // LCD init
  Wire.begin(21, 4); // SDA, SCL
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  // Wi-Fi connect
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print(".");
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");

  // Time setup (India Standard Time)
  configTime(19800, 0, "pool.ntp.org", "time.nist.gov"); // 19800 = GMT+5:30

  // RC522 init
  SPI.begin(18, 19, 23, SS_PIN); // SCK, MISO, MOSI, SS
  rfid.PCD_Init();
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan your card");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  // Read UID
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toLowerCase(); // Normalize UID

  // Get name
  String name = getNameFromUID(uid);

  // Get date and time
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  char dateStr[11];  // DD/MM/YYYY
  char timeStr[9];   // HH:MM:SS
  strftime(dateStr, sizeof(dateStr), "%d/%m/%Y", &timeinfo);
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);

  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("UID:");
  lcd.setCursor(5, 0);
  lcd.print(uid);
  lcd.setCursor(0, 1);
  lcd.print("Name:");
  lcd.setCursor(6, 1);
  lcd.print(name);

  // Send to Google Sheets
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String fullURL = String(scriptURL) +
                     "?UID=" + uid +
                     "&DATE=" + dateStr +
                     "&TIME=" + timeStr +
                     "&NAME=" + name;

    http.begin(fullURL);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String response = http.getString();
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error sending data: " + String(httpCode));
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }

  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan your card");
  rfid.PICC_HaltA();
}