
// Blynk configuration
#define BLYNK_TEMPLATE_ID "TMPL3TzpEB30w"
#define BLYNK_TEMPLATE_NAME "Temperature and Humidity Monitoring"
#define BLYNK_AUTH_TOKEN "9wuen1NcyeiN8B6HaLYIvLrdgvR-AuK4"

#define LED_BUILTIN 2  // esp32 board led pin

#define BLYNK_PRINT Serial

#include "DHT.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>



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

// BME680 sensor (I2C)
Adafruit_BME680 bme;

// Timer for periodic data transmission
BlynkTimer timer;

// Function to read and send sensor data to Blynk
void sendSensorData() {
  // DHT22
  float dht_temp = dht.readTemperature();
  float dht_hum = dht.readHumidity();
  if (!isnan(dht_temp) && !isnan(dht_hum)) {
    Blynk.virtualWrite(V0, dht_temp);
    Blynk.virtualWrite(V1, dht_hum);
  }

  // BME680
  if (bme.performReading()) {
    float bme_temp = bme.temperature;
    float bme_hum = bme.humidity;
    Blynk.virtualWrite(V3, bme_temp);
    Blynk.virtualWrite(V4, bme_hum);
  }

  // DS18B20
  ds18b20.requestTemperatures();
  float ds_temp = ds18b20.getTempCByIndex(0);
  if (ds_temp != DEVICE_DISCONNECTED_C) {
    Blynk.virtualWrite(V2, ds_temp);
  }

  // Debug Output
  Serial.print("DHT22 Temp: "); Serial.print(dht_temp); Serial.print(" °C  ");
  Serial.print("Humidity: "); Serial.println(dht_hum);

  Serial.print("BME680 Temp: "); Serial.print(bme.temperature); Serial.print(" °C  ");
  Serial.print("Humidity: "); Serial.println(bme.humidity);

  Serial.print("DS18B20 Temp: "); Serial.print(ds_temp); Serial.println(" °C");

  //if (dht_temp > 30.0) {
  //Blynk.logEvent("high_temp", "Temperature is too high!");

//}

}

void setup() {
  Serial.begin(115200);

  // Check start for battery opperated mode
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // LED ON for 2 seconds = running
  delay(2000);
  digitalWrite(LED_BUILTIN, LOW);  // LED OFF = reached main setup

  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  


  // Start Blynk & sensors
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  ds18b20.begin();

  if (!bme.begin()) {
    Serial.println("Could not find BME680 sensor!");
    while (1);
  }

  // BME680 settings
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);

  // Start periodic updates every 3 seconds
  timer.setInterval(3000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}