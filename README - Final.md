# HydraSense — TRL-8 Validated Cold Chain Monitoring

**Tagline:** Ensuring medical integrity — from lab to last-mile.

HydraSense is an affordable, India-made cold-chain monitoring solution providing continuous temperature and humidity monitoring to detect unnoticed “silent failures” in vaccine and medicine storage.

## Problem
Every year, life‑saving vaccines, medicines, and organs are wasted due to unnoticed cooling failures. These silent failures put patients at risk and result in massive resource wastage.

## Solution
HydraSense monitors temperature and humidity in real time, pushes data to the cloud, and alerts stakeholders when anomalies occur—ensuring safe storage from lab to last‑mile.

## Key Features
- Real‑time temperature & humidity monitoring  
- Cloud dashboard with historical trends (BLYNK)
- Threshold‑based alerts  
- Modular sensor stack  
- Validated with real‑world durability tests (TRL‑8)

## Sensors Used
- DHT22  
- SHT31‑D  
- DS18B20  
- PT100 RTD  
- BME680  
- SEN54  

## Validation & Testing
- 16‑hour drift test  
- High & low humidity calibration  
- Cold‑to‑hot transition tests  
- Repeated power on/off cycling  
- Real‑world simulation for reliability (TRL‑8 validation)

## System Architecture
```
Sensors → MCU Logger → Connectivity Layer → BLYNK Cloud Dashboard → Alerts
```

## Setup & Usage
1. Connect sensors to MCU (ESP32).  
2. Flash firmware from this repo.  
3. Configure Wi‑Fi & cloud endpoint in `config.h`.  
4. Deploy device inside cold‑chain storage.  
5. View readings on the BLYNK cloud dashboard.

## Roadmap
- Multi‑location deployments  
- Partnerships (B2B/B2C)  
- Predictive maintenance using ML  
- Production‑grade enclosure design  

## Contributors
[Likith Kasam - likithkasam2006@gmail.com
Taral Sanka - taral.sanka@gmail.com
Vinay Kusumanchi - Kusumanchivinay@gmail.com]

## License
MIT License - See [LICENSE](LICENSE) file for details

This project is free to use for educational and research purposes.
