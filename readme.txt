 HYDRA SENSE

TRL-8 Validation and Characterization of DHT22 and DS18B20 Sensors

Team Name: Kanyarashi  
Institution: IIIT Bangalore  
Project for: ELCIA-Sensor Hackathon

---

Overview

Hydra Sense addresses a critical healthcare challenge — unnoticed hydration imbalance. Vulnerable populations such as industrial workers, the elderly, and patients recovering from surgery often experience dehydration-related symptoms that go undetected until too late.

This project validates affordable temperature and humidity sensors to create a reliable, real-time hydration monitoring system suitable for integration into wearables and ambient health systems.

---

Target Application

> Hydra Track — a smart hydration & electrolyte monitoring system for continuous ambient and wearable health assessment.

---

Sensors and Compute Platform

| Sensor / Module     | Purpose                            |
|---------------------|------------------------------------|
| DHT22            | Measures ambient humidity          |
| DS18B20         | Measures skin-contact temperature  |
| BME280 (Reference) | High-accuracy humidity/temperature calibration |
| ESP32-WROOM     | Wireless communication & processing |

---

TRL-8 Readiness Goals

To achieve TRL-8, we performed the following:

- Multi-step calibration of DHT22 and DS18B20 using:
  - Certified hygrometer and thermometer
- Accuracy targets:
  - ±2% RH (humidity)
  - ±0.5°C (temperature)
- Environmental stress tests:
  - Temperature cycling (15–35°C)
  - Simulated sweat exposure
  - Airflow disturbances
- Firmware features:
  - Sensor error detection & recovery
  - Data filtering and compensation algorithms
- Data validation:
  - Calibration curves and correction lookup tables
  - 48-hour drift logs and cross-sensor consistency
- Protective enhancements:
  - Hydrophobic membranes to stabilize readings

---



