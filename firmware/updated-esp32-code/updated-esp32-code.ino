// Blynk configuration
#define BLYNK_TEMPLATE_ID "TMPL3TzpEB30w"
#define BLYNK_TEMPLATE_NAME "Temperature and Humidity Monitoring"
#define BLYNK_AUTH_TOKEN "9wuen1NcyeiN8B6HaLYIvLrdgvR-AuK4"

#define LED_BUILTIN 2
#define BLYNK_PRINT Serial

#include "DHT.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <HTTPClient.h>

// WiFi credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Oneplus Nord CE3 5G";
char pass[] = "12345678";

// DHT22 setup
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// DS18B20 setup
#define ONE_WIRE_BUS 15
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);

// BME680 setup
Adafruit_BME680 bme;

// Timer for Blynk
BlynkTimer timer;

// Telegram Bot Setup
String botToken = "7658515110:AAHKwiabtbTb8RzNEarn1UF2XoxjhRIHvw8";
String chatId = "5117318279";

// Thresholds
const float TEMP_THRESHOLD = 20.0;
const float HUMIDITY_THRESHOLD = 40.0;
const unsigned long alertCooldown = 120000;  // 2 minutes

// State variables
unsigned long lastTempAlertTime = 0;
unsigned long lastHumAlertTime = 0;
bool tempInitialAlertSent = false;
bool humInitialAlertSent = false;
bool systemInitialized = false;

// URL encoding helper
String urlEncode(const String& str) {
  String encoded = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (isalnum(c)) {
      encoded += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) code1 = (c & 0xf) - 10 + 'A';
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) code0 = c - 10 + 'A';
      encoded += '%';
      encoded += code0;
      encoded += code1;
    }
  }
  return encoded;
}

// Sends Telegram message (returns true if success)
bool sendTelegramMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String encodedMessage = urlEncode(message);
    String url = "https://api.telegram.org/bot" + botToken + "/sendMessage?chat_id=" + chatId + "&text=" + encodedMessage;

    Serial.println("[Telegram] Sending: " + message);
    http.begin(url);
    int httpCode = http.GET();
    Serial.print("[Telegram] HTTP code: ");
    Serial.println(httpCode);
    http.end();

    if (httpCode == 200) {
      Serial.println("[Telegram] Message sent successfully");
      return true;
    } else {
      Serial.println("[Telegram] Failed to send message");
      return false;
    }
  } else {
    Serial.println("[Telegram] WiFi not connected");
    return false;
  }
}

// Checks and send alerts
void checkAndSendAlerts(float temp, float humidity) {
  unsigned long currentTime = millis();

  // Temperature alert
  if (temp > TEMP_THRESHOLD) {
    bool shouldSendTempAlert = false;
    if (!tempInitialAlertSent || (currentTime - lastTempAlertTime > alertCooldown)) {
      shouldSendTempAlert = true;
      tempInitialAlertSent = true;
    }
    if (shouldSendTempAlert) {
      bool sent = sendTelegramMessage("🔥 Temp Alert: DS18B20 = " + String(temp, 1) + " °C (Threshold: " + String(TEMP_THRESHOLD, 1) + " °C)");
      if (sent) lastTempAlertTime = millis();
      delay(1000);
    }
  } else {
    tempInitialAlertSent = false;
  }

  // Humidity alert
  if (humidity > HUMIDITY_THRESHOLD) {
    bool shouldSendHumAlert = false;
    if (!humInitialAlertSent || (currentTime - lastHumAlertTime > alertCooldown)) {
      shouldSendHumAlert = true;
      humInitialAlertSent = true;
    }
    if (shouldSendHumAlert) {
      bool sent = sendTelegramMessage("💧 Humidity Alert: BME680 = " + String(humidity, 1) + " % (Threshold: " + String(HUMIDITY_THRESHOLD, 1) + " %)");
      if (sent) lastHumAlertTime = millis();
      delay(1000);
    }
  } else {
    humInitialAlertSent = false;
  }
}

// Read sensors and send to Blynk
void sendSensorData() {
  float dht_temp = dht.readTemperature();
  float dht_hum = dht.readHumidity();

  ds18b20.requestTemperatures();
  float ds_temp = ds18b20.getTempCByIndex(0);

  float bme_temp = NAN, bme_hum = NAN;
  if (bme.performReading()) {
    bme_temp = bme.temperature;
    bme_hum = bme.humidity;
  }

  if (!isnan(dht_temp)) Blynk.virtualWrite(V0, dht_temp);
  if (!isnan(dht_hum)) Blynk.virtualWrite(V1, dht_hum);
  if (ds_temp != DEVICE_DISCONNECTED_C) Blynk.virtualWrite(V2, ds_temp);
  if (!isnan(bme_temp)) Blynk.virtualWrite(V3, bme_temp);
  if (!isnan(bme_hum)) Blynk.virtualWrite(V4, bme_hum);

  if (!isnan(ds_temp) && !isnan(bme_hum)) {
    checkAndSendAlerts(ds_temp, bme_hum);
  } else {
    Serial.println("⚠ DS18B20 readings invalid");
  }
}

// First-time check and status alert
void performInitialCheck() {
  delay(2000);
  float dht_temp = dht.readTemperature();
  float dht_hum = dht.readHumidity();

  if (!isnan(dht_temp) && !isnan(dht_hum)) {
    sendTelegramMessage("✅ ESP32 online and monitoring");
    delay(1000);
    String msg = "📊 Current readings:\n🌡 Temp: " + String(dht_temp, 1) + "°C\n💧 Humidity: " + String(dht_hum, 1) + "%";
    sendTelegramMessage(msg);
    delay(1000);
    checkAndSendAlerts(dht_temp, dht_hum);
  } else {
    sendTelegramMessage("⚠ Sensor read failed - DHT22 not responding");
  }
}

// Setup
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
  digitalWrite(LED_BUILTIN, LOW);

  dht.begin();
  ds18b20.begin();

  if (!bme.begin()) {
    Serial.println("❌ BME680 not found");
  } else {
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
  }

  Blynk.begin(auth, ssid, pass);
  while (WiFi.status() != WL_CONNECTED) delay(100);
  while (!Blynk.connected()) {
    Blynk.run();
    delay(100);
  }

  performInitialCheck();
  timer.setInterval(3000L, sendSensorData);
  systemInitialized = true;
}

// Loop
void loop() {
  Blynk.run();
  timer.run();
  digitalWrite(LED_BUILTIN, (systemInitialized && WiFi.status() == WL_CONNECTED) ? LOW : HIGH);
}