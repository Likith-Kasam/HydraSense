# Sensor Validation Project - TRL-8 Level

---

## Summary

This project validates two low-cost environmental sensors - DHT22 (for humidity and temperature) and DS18B20 (for temperature) - using the BME680 and a certified hygrometer as a reference. The goal is to reach Technology Readiness Level 8 (TRL-8) by testing the sensors across various real-world and controlled environments and documenting their performance, drift, response time, and accuracy. We strictly adhered to the hackathon theme of sensor testing and validation, focusing solely on evaluating commercially available low-cost sensors through systematic and reproducible experiments.

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

1. *Hardware Used*
- ESP32 development board
- DHT22 sensor (connected to GPIO 4)
- DS18B20 sensor (connected to GPIO 15)
- BME680 sensor (I2C: SDA = GPIO 21, SCL = GPIO 22)
- Breadboard, jumper wires, USB cable
- Basic materials: salt (NaCl), silica gel packets, plastic container, cooler (fan), thermos water bottle.

---

2. *Test Setups*

- Ambient Logging (16 hrs) - All three sensors were left connected and running in an indoor room environment for over 16 hours to monitor long-term drift and stability. 
- High-Humidity Calibration - DHT22 and BME680 were placed in a sealed bottle containing a saturated NaCl (salt slurry) solution. Wires were passed through a small hole in the lid. The internal RH was allowed to stabilize around ~75%. 
- Low-Humidity Calibration - Sensors were placed in a sealed bottle along with fresh silica gel sachets. Over time, the internal humidity dropped to ~30%, allowing low-end RH calibration. 
- Thermal Transition Test - DS18B20 was put inside a bottle. The water was replaced sequentially with cold, room-temperature, and hot water. DHT22 and BME680 were suspended inside the bottle to record ambient response to temperature changes.
- Airflow / Fan Test - The entire sensor setup was placed in front of a room cooler's fan. Sudden airflow caused noticeable RH fluctuation, allowing us to observe sensor response to environmental changes. 
- Breath Test - Sensors were briefly exposed to human breath from close range. The resulting RH spike and its recovery time were recorded and analyzed. 
- Power-Cycle Test - The ESP32 board was unplugged and plugged back in during logging. The system's ability to reinitialize all sensors without manual reset was confirmed. 

---

3. *Software & IoT Setup*

- *Arduino IDE* with the following libraries:
  - Adafruit BME680
  - Adafruit Unified Sensor
  - DHT sensor library
  - OneWire and DallasTemperature
  - Blynk IoT platform library

- *Blynk IoT Configuration*:
  - Created Blynk project with template ID: TMPL3TzpEB30w
  - Configured dashboard widgets to display:
    - Virtual Pin V0: DHT22 Temperature
    - Virtual Pin V1: DHT22 Humidity
    - Virtual Pin V2: DS18B20 Temperature
    - Virtual Pin V3: BME680 Temperature
    - Virtual Pin V4: BME680 Humidity
  - Set up 3-second update interval for all sensors
  - Enabled serial debug output for local monitoring

- *Firmware Features*:
  - Automatic WiFi connection to "Iqoo 7" network
  - Sensor initialization checks
  - BME680 oversampling configuration (8X for temp, 2X for humidity)
  - Built-in LED status indicator (2 sec ON = startup complete)
  - Robust error handling for disconnected sensors

- *Data Collection*:
  - Python logger script used to capture and store data in .csv format
  - Microsoft Excel used for plotting and analysis of time vs RH/temp graphs
  - Simultaneous cloud monitoring via Blynk mobile/desktop app

### *Indian Sensor Replacements*  

|  *Original Sensor*  | *Indian Alternative* |    *Manufacturer*      |      *Key Features*           | *Where to Buy* |
|--------------------|----------------------|------------------------|-------------------------------|----------------|

|DHT22(Temp/Humidity)| SHT31-D(I2C)         | Enviro Solutions India | ±2% RH, ±0.2°C, I2C interface | [Robu.in](https://robu.in/) /|
												       [ElectronicsComp](https://www.electronicscomp.com/)|
|DS18B20(Temp)       | PT100 RTD +          | Raghav Industries      | High-accuracy,industrial-grade| [Tessol India](https://www.tessol.in/) |
		       Signal Conditioner
|BME680              |SEN54(Sensirion-based)| Enviro Solutions India |Temp, Humidity, VOC, PM2.5/PM10| [ProtoCentral](https://protocentral.com/)|	
 (Temp/Humidity/Gas)													  

---

### *Why These Alternatives?*
1. *SHT31-D (Replacement for DHT22)*  
   - More accurate (±2% RH vs DHT22’s ±5%)  
   - I2C interface (better than DHT22’s single-wire)  
   - Made by *Sensirion*, assembled in India  

2. *PT100 RTD (Replacement for DS18B20)*  
   - Higher accuracy (±0.1°C possible)  
   - Industrial-grade reliability  
   - Requires an ADC (ADS1115) but better for harsh environments  

3. *SEN54 (Replacement for BME680)*  
   - Adds *particulate matter (PM2.5/PM10)* sensing  
   - Same Temp/Humidity accuracy as BME680  
   - Made under license from *Sensirion (Swiss tech, Indian assembly)*
  
###  *Future Proposals*

### 1. *Integration with Indigenous Sensors*

> Propose replacing DHT22 and DS18B20 with *Made-in-India sensors* (e.g., Sensironics, SHT318 equivalents) to support Atmanirbhar Bharat and reduce import dependency.

* Compare performance, cost, and compatibility.
* Help validate and promote Indian IoT hardware ecosystem.

---

### 2. *Real-Time Alerts & Decision System*

> Add threshold-based *alerts or control systems* triggered by sensor readings (e.g., buzzer for high humidity, fan activation, or mobile notifications).

* Extend the current Blynk setup to include smart rules.
* Improve practicality in agriculture, home automation, or weather monitoring.

---

### 3. *Field Deployment & Weatherproof Enclosure*

> Transition from lab tests to *outdoor field trials* by designing a weatherproof enclosure.

* Use 3D-printed or IP65-rated boxes.
* Power it with a *battery + solar module* for continuous off-grid operation.

---

### 4. *Machine Learning for Sensor Correction*

> Use the reference sensor data to train a model that *auto-calibrates* low-cost sensors in software.

* Use simple regression or ML techniques to predict corrected values.
* Implement on-device (ESP32) or cloud-based correction pipeline.

---

### 5. *Multi-Node Sensor Network*

> Deploy *multiple sensor nodes* in different rooms or locations and aggregate data centrally.

* Use *ESP-NOW*, LoRa, or MQTT to transmit to a base station.
* Useful for smart homes, greenhouses, and industry environments.

---