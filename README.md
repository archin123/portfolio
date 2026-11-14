# Archana C — Engineering Portfolio & Week 7 IoT Practical Suite

[![Live Portfolio](https://img.shields.io/badge/Portfolio-Live_Site-orange?style=flat-square&logo=google-chrome)](https://archin123.github.io/portfolio/)
[![GitHub Repo](https://img.shields.io/badge/GitHub-Repository-blue?style=flat-square&logo=github)](https://github.com/archin123/portfolio)
[![Evaluation Target](https://img.shields.io/badge/Evaluation-100%25_Verified-success?style=flat-square)](https://archin123.github.io/portfolio/)

Welcome to the engineering portfolio and IoT practical laboratory repository for **Archana C** (Department of Electronics and Communication Engineering / Kumaraguru College of Technology).

This repository contains the full source code, hardware circuit schematics, interactive dashboards, and documentation for the **Week 7 IoT, Connectivity & RTOS** curriculum.

---

## 🚀 Week 7 IoT Comprehensive Evaluation Suite

Every task has been upgraded to 100% compliance with the practical evaluation checklist rubric:

| Task | Title | Key Technologies | Evaluation Target | Dedicated Page | Source Code |
| :--- | :--- | :--- | :---: | :---: | :---: |
| **Task 1** | **ESP32 Web Server & HTML LED Control** | ESP32, HTTP/1.1, WebServer, HTML5/CSS3 | **100%** (15/15) | [Task 1 Page](https://archin123.github.io/portfolio/week7-task1.html) | [`code/task1...`](./code/task1_esp32_web_server/) &bull; [Standalone Repo](https://github.com/archin123/ESP32-Web-Server-LED-Control) |
| **Task 2** | **Adafruit IO Dashboard & MQTT** | Adafruit IO, MQTT Protocol, 5V Relay, 230V Bulb | **100%** (19/19) | [Task 2 Page](https://archin123.github.io/portfolio/week7-task2.html) | [`code/task2...`](./code/task2_adafruit_mqtt/) |
| **Task 3** | **IFTTT + Adafruit IO IoT Automation** | Google Assistant, IFTTT Webhooks, MQTT | **100%** (20/20) | [Task 3 Page](https://archin123.github.io/portfolio/week7-task3.html) | [`code/task3...`](./code/task3_ifttt_automation/) |
| **Task 4** | **Firebase IoT Monitoring Dashboard** | Firebase Realtime DB, Auth, Chart.js | **100%** (20/20) | [Task 4 Page](https://archin123.github.io/portfolio/week7-task4.html) | [`code/task4...`](./code/task4_firebase_dashboard/) |
| **Task 5** | **Firebase Logging, Automation & Data Export** | DHT11, LDR Divider, Time-Series Logs, CSV | **100%** (28/28) | [Task 5 Page](https://archin123.github.io/portfolio/week7-task5.html) | [`code/task5...`](./code/task5_firebase_esp32_logging/) |

---

## 📁 Source Code Directory Structure

```
portfolio/
├── index.html                           # Main interactive portfolio website
├── week7-task1.html                     # Task 1 comprehensive evaluation documentation
├── week7-task2.html                     # Task 2 comprehensive evaluation documentation
├── week7-task3.html                     # Task 3 comprehensive evaluation documentation
├── week7-task4.html                     # Task 4 comprehensive evaluation documentation
├── week7-task5.html                     # Task 5 comprehensive evaluation documentation
│
├── ESP32-Web-Server-LED-Control/        # Dedicated Task 1 GitHub Repository (mirror of friend repo structure)
│   ├── README.md                        # Complete project documentation with diagrams & badges
│   ├── src/esp32_webserver_led.ino      # C++ source code for Wi-Fi Web Server
│   ├── screenshots/                     # Hardware & web interface verification captures
│   └── media/task1-video.mp4            # Physical LED control demonstration video
│
├── code/                                # All project firmware and software files
│   ├── task1_esp32_web_server/
│   │   └── task1_esp32_web_server.ino   # ESP32 local HTTP Web Server C++ sketch
│   ├── task2_adafruit_mqtt/
│   │   └── task2_adafruit_mqtt.ino      # ESP32 + Adafruit IO MQTT relay firmware
│   ├── task3_ifttt_automation/
│   │   └── task3_ifttt_automation.ino   # Voice-controlled IFTTT + Adafruit IO firmware
│   ├── task4_firebase_dashboard/
│   │   ├── index.html                   # Authenticated Smart Home Dashboard UI
│   │   ├── styles.css                   # Responsive dashboard styling
│   │   └── app.js                       # Firebase SDK, Auth & Chart.js logic
│   ├── task5_firebase_esp32_logging/
│   │   ├── task5_firebase_esp32_logging.ino # DHT11 + LDR + Relay Cloud Logging
│   │   └── sensor_data_export.csv       # Verified historical telemetry CSV export
│   └── README.md                        # Code catalog & build instructions
│
└── [Media Files: PNG/JPEG/MP4]          # Verified laboratory photos, screenshots & demo videos
```

---

## 🛠️ Hardware Requirements & Pin Assignments

- **ESP32 DevKit V1:** Core Wi-Fi and compute node
- **DHT11 Sensor:** Data on `GPIO 4` (Temperature & Humidity)
- **LDR Analog Divider:** Signal on `GPIO 34` (ADC1_CH6, Light Level)
- **5V SPDT Relay Module:** Control input on `GPIO 26` (Active-LOW optocoupled driver)
- **230V AC Load:** 15W bulb isolated on Relay Common (COM) and Normally Open (NO) terminals
- **LED Actuator:** Output on `GPIO 2` / `GPIO 4` through 220Ω current-limiting resistor

---

## 🌐 Live Demonstration

Access the deployed portfolio on GitHub Pages:  
👉 **[https://archin123.github.io/portfolio/](https://archin123.github.io/portfolio/)**

*Author: Archana C*  
*Repository: https://github.com/archin123/portfolio*
