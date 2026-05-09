/* ESP32 WiFi Scanning example */
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid = "Wokwi-GUEST";
const char* password = "";

String apiKey = "V1QG2ZPC4LWTRONB";

const int sensorPin = 34;

float mainsVoltage = 230.0;

void setup() {

  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Energy Meter");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  lcd.clear();
  lcd.print("WiFi Connected");

  delay(2000);
}

void loop() {

  int adcValue = analogRead(sensorPin);

  float sensorVoltage = adcValue * (3.3 / 4095.0);

  float current = abs((sensorVoltage - 1.65) * 10);

  float power = mainsVoltage * current;

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("I:");
  lcd.print(current,2);
  lcd.print("A");

  lcd.setCursor(0,1);
  lcd.print("P:");
  lcd.print(power,1);
  lcd.print("W");

  Serial.print("Current: ");
  Serial.println(current);

  Serial.print("Power: ");
  Serial.println(power);

  sendToThingSpeak(current, power);

  delay(30000);
}

void sendToThingSpeak(float current, float power) {

  if(WiFi.status()== WL_CONNECTED){

    HTTPClient http;

    String url = "http://api.thingspeak.com/update?api_key=" 
                 + apiKey 
                 + "&field1=" + String(current)
                 + "&field2=" + String(power);

    http.begin(url);

    int httpResponseCode = http.GET();

    Serial.print("HTTP Response: ");
    Serial.println(httpResponseCode);

    http.end();
  }
}

