# Week 7 IoT Source Code Repository
**Author:** Archana C  
**Institution:** Kumaraguru College of Technology (KCT)  
**Live Portfolio:** [https://archin123.github.io/portfolio/](https://archin123.github.io/portfolio/)  
**Main Repository:** [https://github.com/archin123/portfolio](https://github.com/archin123/portfolio)

This directory contains the verified, documented, and complete source code files for all five Week 7 IoT tasks evaluated according to the practical checklist rubric.

---

## Directory Overview

```
code/
├── task1_esp32_web_server/
│   └── task1_esp32_web_server.ino       # ESP32 local HTTP Web Server & HTML LED Control
├── task2_adafruit_mqtt/
│   └── task2_adafruit_mqtt.ino          # ESP32 + Adafruit IO MQTT 230V Bulb Control
├── task3_ifttt_automation/
│   └── task3_ifttt_automation.ino       # Google Assistant + IFTTT + Adafruit IO Automation
├── task4_firebase_dashboard/
│   ├── index.html                       # Standalone Firebase IoT Monitoring Dashboard UI
│   ├── styles.css                       # Responsive CSS styles matching the project theme
│   └── app.js                           # Firebase Auth, Realtime DB listeners & Chart.js logic
├── task5_firebase_esp32_logging/
│   ├── task5_firebase_esp32_logging.ino # ESP32 DHT11 + LDR + Relay Cloud Logging Firmware
│   └── sensor_data_export.csv           # Verified downloaded CSV export of historical sensor data
└── README.md                            # Code index and documentation guide
```

---

## Task Details & Arduino Dependencies

### Task 1: ESP32 Web Server & HTML LED Control
- **File:** [`task1_esp32_web_server/task1_esp32_web_server.ino`](./task1_esp32_web_server/task1_esp32_web_server.ino)
- **Target Board:** ESP32 Dev Module
- **Core Libraries:** `WiFi.h`, `WebServer.h` (Built into Espressif ESP32 Core)
- **Default Pin:** GPIO 2 (Built-in LED) or GPIO 4 / GPIO 26 (External LED with 220Ω resistor)
- **Key Features:** Embedded HTML5/CSS3 control UI, non-blocking client handling via `server.handleClient()`, dynamic status rendering, Serial Monitor diagnostics at 115200 baud.

### Task 2: Adafruit IO Dashboard & MQTT Cloud Control
- **File:** [`task2_adafruit_mqtt/task2_adafruit_mqtt.ino`](./task2_adafruit_mqtt/task2_adafruit_mqtt.ino)
- **Target Board:** ESP32 Dev Module
- **Core Libraries:** `WiFi.h`, `Adafruit_MQTT.h`, `Adafruit_MQTT_Client.h`
- **Adafruit IO Account:** `archana_07`
- **Subscribed Topic:** `archana_07/feeds/bulb`
- **Hardware Output:** GPIO 26 connected to 5V SPDT Relay module switching 230V AC bulb.
- **Key Features:** Active-LOW relay logic, automatic TCP reconnection with backoff, keepalive pings.

### Task 3: IFTTT + Adafruit IO IoT Automation
- **File:** [`task3_ifttt_automation/task3_ifttt_automation.ino`](./task3_ifttt_automation/task3_ifttt_automation.ino)
- **Target Board:** ESP32 Dev Module
- **Voice Assistant:** Google Assistant ("Okay Google, activate Bulb on" / "Okay Google, Turn the Bulb OFF")
- **Cloud Bridge:** IFTTT Applet -> Adafruit IO Feed `archana_07/feeds/bulb` -> ESP32 MQTT Client
- **Key Features:** Complete voice automation path, connection watchdog, low-latency relay execution.

### Task 4: Firebase IoT Monitoring Dashboard
- **Directory:** [`task4_firebase_dashboard/`](./task4_firebase_dashboard/)
  - `index.html`: Responsive HTML with login card and sensor/appliance monitoring grid.
  - `styles.css`: Dark/light balanced UI matching `task 4 one.png` and `task 4 four.png`.
  - `app.js`: Firebase Realtime Database sync (`esp32-iot-monitor-8d34d`), Authentication session observer, live Chart.js stream.
- **Key Features:** Email/Password Authentication, Realtime Database listeners (`/sensors`, `/control`), interactive Manual/Automatic toggle, dynamic historical table.

### Task 5: Firebase Logging, Automation & Data Export
- **Firmware:** [`task5_firebase_esp32_logging/task5_firebase_esp32_logging.ino`](./task5_firebase_esp32_logging/task5_firebase_esp32_logging.ino)
- **Sensors:** DHT11 Temperature & Humidity (GPIO 4), LDR Light Level (GPIO 34 ADC1)
- **Historical CSV Data:** [`task5_firebase_esp32_logging/sensor_data_export.csv`](./task5_firebase_esp32_logging/sensor_data_export.csv)
- **Key Features:** Dual operating mode (MANUAL via dashboard override / AUTO via calibrated LDR threshold `> 3800`), periodic telemetry write to `/sensors`, immutable timestamped log pushing to `/logs`, browser CSV export.

---
*Maintained by Archana C for the IoT Portfolio Evaluation.*

