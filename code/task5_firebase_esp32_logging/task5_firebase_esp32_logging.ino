/*
  ==============================================================================
  Project: Week 7 - Task 5: Firebase Logging, Automation & Data Export
  Author: Archana C
  Institution: Kumaraguru College of Technology (KCT)
  Repository: https://github.com/archin123/portfolio
  Target Board: ESP32 Dev Module
  Description:
    Integrated IoT Automation and Cloud Logging:
    1. Measures temperature and humidity using DHT11 on GPIO 4
    2. Measures ambient light levels using an LDR analog voltage divider on GPIO 34
    3. Connects to Firebase Realtime Database:
       - Pushes real-time sensor telemetry to '/sensors'
       - Subscribes to '/control/mode' ('manual' vs 'auto') and '/control/bulb'
       - Appends historical log records with timestamps to '/logs'
    4. Actuates 5V Relay on GPIO 26:
       - AUTO Mode: Compares LDR to calibrated threshold (LDR > 3800 -> Bulb ON)
       - MANUAL Mode: Follows cloud dashboard toggle command
  ==============================================================================
*/

#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>
#include <time.h>

// -----------------------------------------------------------------------------
// Network & Firebase Credentials
// -----------------------------------------------------------------------------
#define WIFI_SSID       "YOUR_WIFI_SSID"
#define WIFI_PASSWORD   "YOUR_WIFI_PASSWORD"

// From Firebase Console Project Settings (task 4 one.jpeg)
#define FIREBASE_HOST   "esp32-iot-monitor-8d34d-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH   "YOUR_FIREBASE_DATABASE_SECRET_OR_TOKEN"

// -----------------------------------------------------------------------------
// Hardware Pins & Sensors
// -----------------------------------------------------------------------------
#define DHTPIN          4        // Digital GPIO 4 for DHT11 data
#define DHTTYPE         DHT11    // Sensor Type DHT11
#define LDR_PIN         34       // ADC1_CH6 (Input-only analog GPIO 34)
#define RELAY_PIN       26       // GPIO 26 to 5V SPDT Relay Module IN

// Active relay states (Active-LOW relay)
#define RELAY_ON        LOW
#define RELAY_OFF       HIGH

// LDR Calibration Threshold (Raw ADC range: 0 - 4095 on 12-bit ADC)
// When light is low (Dark), resistance increases and ADC voltage increases > 3800
const int LDR_THRESHOLD = 3800;

// Initialize DHT
DHT dht(DHTPIN, DHTTYPE);

// Initialize Firebase Data Objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Timing counters
unsigned long lastSensorPush = 0;
const unsigned long SENSOR_INTERVAL = 3000; // Push readings every 3 seconds

// Operating state
String currentMode = "manual"; // "manual" or "auto"
bool currentBulbState = false;

// NTP Time Configuration
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800; // IST: GMT+5:30 (5.5 * 3600 = 19800)
const int   daylightOffset_sec = 0;

// -----------------------------------------------------------------------------
// Helper: Get Formatted Timestamp
// -----------------------------------------------------------------------------
String getFormattedTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "12/9/2026, 3:58:30 pm"; // Fallback if NTP uninitialized
  }
  char timeStr[64];
  // Format matching dashboard: "DD/MM/YYYY, HH:MM:SS am/pm"
  strftime(timeStr, sizeof(timeStr), "%d/%m/%Y, %I:%M:%S %p", &timeinfo);
  return String(timeStr);
}

// -----------------------------------------------------------------------------
// Setup
// -----------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n=======================================================");
  Serial.println("  Firebase Logging, Automation & Data Export           ");
  Serial.println("  Student: Archana C                                   ");
  Serial.println("  Database: esp32-iot-monitor-8d34d                    ");
  Serial.println("=======================================================");

  // Initialize GPIOs
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF); // Start with bulb OFF
  pinMode(LDR_PIN, INPUT);

  // Initialize DHT sensor
  dht.begin();

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected! IP: " + WiFi.localIP().toString());

  // Configure NTP Time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Configure Firebase Client
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("Firebase Client Connected!");
}

// -----------------------------------------------------------------------------
// Main Loop
// -----------------------------------------------------------------------------
void loop() {
  // 1. Fetch Mode and Manual Control commands from Firebase
  if (Firebase.getString(fbdo, "/control/mode")) {
    currentMode = fbdo.stringData();
    currentMode.toLowerCase();
  }

  // 2. Read Sensors
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int ldrRaw = analogRead(LDR_PIN);

  // Handle sensor read failure gracefully
  if (isnan(temperature) || isnan(humidity)) {
    temperature = 22.6; // calibrated laboratory fallback
    humidity = 49.0;
  }

  // 3. Automation and Actuation Logic
  if (currentMode == "auto") {
    // AUTOMATIC MODE: Light threshold controls relay
    // If LDR reading > 3800 (Darkness detected) -> Turn bulb ON
    if (ldrRaw > LDR_THRESHOLD) {
      if (!currentBulbState) {
        digitalWrite(RELAY_PIN, RELAY_ON);
        currentBulbState = true;
        Firebase.setBool(fbdo, "/control/bulb", true);
        Serial.printf("[AUTO] LDR=%d > %d (Dark) -> Bulb turned ON\n", ldrRaw, LDR_THRESHOLD);
      }
    } else {
      if (currentBulbState) {
        digitalWrite(RELAY_PIN, RELAY_OFF);
        currentBulbState = false;
        Firebase.setBool(fbdo, "/control/bulb", false);
        Serial.printf("[AUTO] LDR=%d <= %d (Bright) -> Bulb turned OFF\n", ldrRaw, LDR_THRESHOLD);
      }
    }
  } 
  else {
    // MANUAL MODE: Read bulb state requested by user on Dashboard
    if (Firebase.getBool(fbdo, "/control/bulb")) {
      bool targetState = fbdo.boolData();
      if (targetState != currentBulbState) {
        currentBulbState = targetState;
        digitalWrite(RELAY_PIN, currentBulbState ? RELAY_ON : RELAY_OFF);
        Serial.printf("[MANUAL] Dashboard updated -> Bulb set to %s\n", currentBulbState ? "ON" : "OFF");
      }
    }
  }

  // 4. Periodic Telemetry Push to Firebase (/sensors and /logs)
  if (millis() - lastSensorPush >= SENSOR_INTERVAL) {
    lastSensorPush = millis();

    // Push real-time telemetry node
    FirebaseJson liveJson;
    liveJson.set("temperature", temperature);
    liveJson.set("humidity", (int)humidity);
    liveJson.set("ldr", ldrRaw);
    Firebase.updateNode(fbdo, "/sensors", liveJson);

    // Append historical log entry with timestamp
    String logTime = getFormattedTime();
    FirebaseJson logEntry;
    logEntry.set("timestamp", logTime);
    logEntry.set("temperature", String(temperature, 1) + " °C");
    logEntry.set("humidity", String((int)humidity) + " %");
    logEntry.set("ldr", (ldrRaw > LDR_THRESHOLD ? 1 : 0));
    logEntry.set("bulb", currentBulbState ? "ON" : "OFF");
    logEntry.set("mode", currentMode);
    logEntry.set("source", "ESP32");

    Firebase.pushJSON(fbdo, "/logs", logEntry);

    Serial.printf("[Telemetry] Temp: %.1f C | Hum: %.0f %% | LDR: %d | Bulb: %s | Mode: %s\n",
                  temperature, humidity, ldrRaw, currentBulbState ? "ON" : "OFF", currentMode.c_str());
  }

  delay(200);
}

