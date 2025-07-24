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
char ssid[] = "Iqoo 7";
char pass[] = "123456789@";

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

// Thresholds for alerts
const float TEMP_THRESHOLD = 20.0;
const float HUMIDITY_THRESHOLD = 40.0;
const float DHT_TEMP_THRESHOLD = 70.0;
const unsigned long alertCooldown = 120000;  // 2 minutes

// State variables
unsigned long lastTempAlertTime = 0;
unsigned long lastHumAlertTime = 0;
unsigned long lastDHTTempAlertTime = 0;
bool tempInitialAlertSent = false;
bool humInitialAlertSent = false;
bool dhtTempInitialAlertSent = false;
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

// Send Telegram message (returns true if success)
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

// Check and send alerts
void checkAndSendAlerts(float ds_temp, float bme_hum, float dht_temp) {
  unsigned long currentTime = millis();

  // Temperature alert from DS18B20
  if (ds_temp > TEMP_THRESHOLD) {
    bool shouldSendTempAlert = false;
    if (!tempInitialAlertSent || (currentTime - lastTempAlertTime > alertCooldown)) {
      shouldSendTempAlert = true;
      tempInitialAlertSent = true;
    }
    if (shouldSendTempAlert) {
      bool sent = sendTelegramMessage("Temp Alert: DS18B20 = " + String(ds_temp, 1) + " °C (Threshold: " + String(TEMP_THRESHOLD, 1) + " °C)");
      if (sent) lastTempAlertTime = millis();
      delay(1000);
    }
  } else {
    tempInitialAlertSent = false;
  }

  // Humidity alert from BME680
  if (bme_hum > HUMIDITY_THRESHOLD) {
    bool shouldSendHumAlert = false;
    if (!humInitialAlertSent || (currentTime - lastHumAlertTime > alertCooldown)) {
      shouldSendHumAlert = true;
      humInitialAlertSent = true;
    }
    if (shouldSendHumAlert) {
      bool sent = sendTelegramMessage("Humidity Alert: BME680 = " + String(bme_hum, 1) + " % (Threshold: " + String(HUMIDITY_THRESHOLD, 1) + " %)");
      if (sent) lastHumAlertTime = millis();
      delay(1000);
    }
  } else {
    humInitialAlertSent = false;
  }

  // DHT22 temperature alert
  if (dht_temp > DHT_TEMP_THRESHOLD) {
    bool shouldSendDHTTempAlert = false;
    if (!dhtTempInitialAlertSent || (currentTime - lastDHTTempAlertTime > alertCooldown)) {
      shouldSendDHTTempAlert = true;
      dhtTempInitialAlertSent = true;
    }
    if (shouldSendDHTTempAlert) {
      bool sent = sendTelegramMessage("⚠ DHT22 Temp Alert: " + String(dht_temp, 1) + " °C (Threshold: " + String(DHT_TEMP_THRESHOLD, 1) + " °C)");
      if (sent) lastDHTTempAlertTime = millis();
      delay(1000);
    }
  } else {
    dhtTempInitialAlertSent = false;
  }
}

// Read sensors and send to Blynk
void sendSensorData() {
  float dht_temp = dht.readTemperature()+0.53;
  float dht_hum = dht.readHumidity()-2.27;

  ds18b20.requestTemperatures();
  float ds_temp = ds18b20.getTempCByIndex(0);

  float bme_temp = NAN, bme_hum = NAN;
  if (bme.performReading()) {
    bme_temp = bme.temperature;
    bme_hum = bme.humidity;
  }

  // Send all readings to Blynk
  if (!isnan(dht_temp)) Blynk.virtualWrite(V0, dht_temp);
  if (!isnan(dht_hum)) Blynk.virtualWrite(V1, dht_hum);
  if (ds_temp != DEVICE_DISCONNECTED_C) Blynk.virtualWrite(V2, ds_temp);
  if (!isnan(bme_temp)) Blynk.virtualWrite(V3, bme_temp);
  if (!isnan(bme_hum)) Blynk.virtualWrite(V4, bme_hum);

  if (ds_temp != DEVICE_DISCONNECTED_C && !isnan(bme_hum) && !isnan(dht_temp)) {
    checkAndSendAlerts(ds_temp, bme_hum, dht_temp);
  } else {
    Serial.println("⚠ DS18B20, DHT22 or BME680 reading invalid");
  }
}

// First-time status alert
void performInitialCheck() {
  delay(2000);
  ds18b20.requestTemperatures();
  float ds_temp = ds18b20.getTempCByIndex(0);

  float bme_hum = NAN;
  if (bme.performReading()) {
    bme_hum = bme.humidity;
  }

  if (ds_temp != DEVICE_DISCONNECTED_C && !isnan(bme_hum)) {
    sendTelegramMessage("ESP32 online and monitoring DS18B20 & BME680");
    delay(1000);
    String msg = "Current readings:\n Temp (DS18B20): " + String(ds_temp, 1) + "°C\n Humidity (BME680): " + String(bme_hum, 1) + "%";
    sendTelegramMessage(msg);
    delay(1000);
    float dht_temp = dht.readTemperature();
    checkAndSendAlerts(ds_temp, bme_hum, dht_temp);
  } else {
    sendTelegramMessage("⚠ Initial sensor read failed - DS18B20 or BME680 not responding");
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
    Serial.println("BME680 not found");
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