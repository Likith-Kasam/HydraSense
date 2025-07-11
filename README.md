# Sensor Validation Project - TRL-8 Level

---

## Summary

This project validates two low-cost environmental sensors - DHT22 (for humidity and temperature) and DS18B20 (for temperature) - using the BME680 and a certified hygrometer as a reference. The goal is to reach Technology Readiness Level 8 (TRL-8) by testing the sensors across various real-world and controlled environments and documenting their performance, drift, response time, and accuracy.

The ESP32-based system logs data from all three sensors under different conditions and transmits real-time data to the Blynk IoT platform:
- Long-duration ambient monitoring
- High-humidity (salt slurry) and low-humidity (silica gel) calibration
- Thermal transition response (cold to hot)
- Fan and breath-based humidity variation tests
- Power-cycle robustness check

All data is saved, plotted, and analyzed to confirm TRL-8 readiness while being simultaneously available on both local storage and cloud visualization via Blynk.

---

## Sensors Used

| Sensor | Measures              | Notes                             |
|--------|-----------------------|-----------------------------------|
| DHT22  | Temperature, Humidity | Ambient sensor, RH drift corrected|
| DS18B20| Temperature (1-wire)  | Contact sensor, fast response     |
| BME680 | Temp, Humidity        | Reference sensor                  |

---

## TRL-8 Goals

### Goal - Description of What We Did 

Long-Term Stability
High-Humidity Calibration 
Low-Humidity Calibration
Thermal Response Test
Dynamic Humidity Test
Power-Cycle Recovery
Data Logging, Graphing & Repo
Cloud Monitoring via Blynk

---

## Setup Steps

1. Hardware Used
- ESP32 development board
- DHT22 sensor (connected to GPIO 4)
- DS18B20 sensor (connected to GPIO 15 with 4.7kΩ pull-up resistor)
- BME680 sensor (I2C: SDA = GPIO 21, SCL = GPIO 22)
- Breadboard, jumper wires, USB cable
- Basic materials: salt (NaCl), silica gel packets, plastic container, cooler (fan), thermos water bottle.

---

2. Test Setups

- Ambient Logging (16 hrs) - All three sensors were left connected and running in an indoor room environment for over 16 hours to monitor long-term drift and stability. 
- High-Humidity Calibration - DHT22 and BME680 were placed in a sealed plastic box containing a saturated NaCl (salt slurry) solution. Wires were passed through a small hole in the lid. The internal RH was allowed to stabilize around ~75%. 
- Low-Humidity Calibration - Sensors were placed in a sealed box along with fresh silica gel sachets. Over time, the internal humidity dropped to ~30%, allowing low-end RH calibration. 
- Thermal Transition Test - DS18B20 was taped to the side of a plastic water bottle. The water was replaced sequentially with cold, room-temperature, and hot water. DHT22 and BME680 were suspended inside the bottle to record ambient response to temperature changes.
- Airflow / Fan Test - The entire sensor setup was placed in front of a room cooler's fan. Sudden airflow caused noticeable RH fluctuation, allowing us to observe sensor response to environmental changes. 
- Breath Test - Sensors were briefly exposed to human breath from close range. The resulting RH spike and its recovery time were recorded and analyzed. 
- Power-Cycle Test - The ESP32 board was unplugged and plugged back in during logging. The system's ability to reinitialize all sensors without manual reset was confirmed. 

---

3. Software & IoT Setup

- **Arduino IDE** with the following libraries:
  - Adafruit BME680
  - Adafruit Unified Sensor
  - DHT sensor library
  - OneWire and DallasTemperature
  - Blynk IoT platform library

- **Blynk IoT Configuration**:
  - Created Blynk project with template ID: `TMPL3TzpEB30w`
  - Configured dashboard widgets to display:
    - Virtual Pin V0: DHT22 Temperature
    - Virtual Pin V1: DHT22 Humidity
    - Virtual Pin V2: DS18B20 Temperature
    - Virtual Pin V3: BME680 Temperature
    - Virtual Pin V4: BME680 Humidity
  - Set up 20-second update interval for all sensors
  - Enabled serial debug output for local monitoring

- **Firmware Features**:
  - Automatic WiFi connection to "Iqoo 7" network
  - Sensor initialization checks
  - BME680 oversampling configuration (8X for temp, 2X for humidity)
  - Built-in LED status indicator (2 sec ON = startup complete)
  - Robust error handling for disconnected sensors

- **Data Collection**:
  - Python logger script used to capture and store data in .csv format
  - Microsoft Excel used for plotting and analysis of time vs RH/temp graphs
  - Simultaneous cloud monitoring via Blynk mobile/desktop app

---