/*
  ==============================================================================
  Project: Week 7 - Task 3: IFTTT + Adafruit IO IoT Automation
  Author: Archana C
  Institution: Kumaraguru College of Technology (KCT)
  Repository: https://github.com/archin123/portfolio
  Target Board: ESP32 Dev Module
  Description:
    Voice-controlled cloud automation pipeline:
    1. User speaks command ("Okay Google, activate Bulb on" / "Turn the Bulb OFF")
    2. Google Assistant triggers IFTTT Applet
    3. IFTTT Action publishes payload ("ON" / "OFF") to Adafruit IO feed 'bulb'
    4. Adafruit IO MQTT Broker pushes message to subscribed ESP32
    5. ESP32 decodes payload and actuates 5V relay module on GPIO 26
  ==============================================================================
*/

#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// -----------------------------------------------------------------------------
// Network & Adafruit IO Configuration
// -----------------------------------------------------------------------------
#define WLAN_SSID       "YOUR_WIFI_SSID"
#define WLAN_PASS       "YOUR_WIFI_PASSWORD"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "archana_07"
#define AIO_KEY         "aio_YOUR_SECRET_AIO_KEY"

// Hardware Output Pin
const int RELAY_PIN = 26;
const bool RELAY_ON  = LOW;   // Active-LOW relay
const bool RELAY_OFF = HIGH;

// State tracking
bool currentBulbState = false;
unsigned long lastKeepAlive = 0;

// MQTT Client & Subscription
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe bulbFeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/bulb");

// -----------------------------------------------------------------------------
// Function Prototypes
// -----------------------------------------------------------------------------
void connectWiFi();
void connectMQTT();

// -----------------------------------------------------------------------------
// Callback triggered when IFTTT updates the Adafruit IO Feed
// -----------------------------------------------------------------------------
void onBulbFeedUpdate(char *data, uint16_t len) {
  String command = String(data);
  command.trim();

  Serial.println("\n----------------------------------------");
  Serial.print("[IFTTT -> Adafruit IO -> ESP32] Voice Action Received: ");
  Serial.println(command);

  if (command.equalsIgnoreCase("ON") || command == "1" || command.equalsIgnoreCase("activate")) {
    digitalWrite(RELAY_PIN, RELAY_ON);
    currentBulbState = true;
    Serial.println("Action Executed -> Relay Pin 26: LOW (Enclosed AC Bulb: ON)");
  } 
  else if (command.equalsIgnoreCase("OFF") || command == "0" || command.equalsIgnoreCase("deactivate")) {
    digitalWrite(RELAY_PIN, RELAY_OFF);
    currentBulbState = false;
    Serial.println("Action Executed -> Relay Pin 26: HIGH (Enclosed AC Bulb: OFF)");
  } 
  else {
    Serial.print("Unhandled command format: ");
    Serial.println(command);
  }
  Serial.println("----------------------------------------");
}

// -----------------------------------------------------------------------------
// Setup
// -----------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n=======================================================");
  Serial.println("  IFTTT + Adafruit IO Voice Automation System         ");
  Serial.println("  Student: Archana C | Device: ESP32 + Relay (GPIO26) ");
  Serial.println("=======================================================");

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF); // Safe initial state
  currentBulbState = false;

  connectWiFi();

  // Attach callback and subscribe to the feed
  bulbFeed.setCallback(onBulbFeedUpdate);
  mqtt.subscribe(&bulbFeed);

  Serial.println("[MQTT] Subscribed to voice action feed: " AIO_USERNAME "/feeds/bulb");
}

// -----------------------------------------------------------------------------
// Loop
// -----------------------------------------------------------------------------
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  connectMQTT();

  // Listen for IFTTT incoming updates through Adafruit IO broker
  mqtt.processPackets(4000);

  // Ping broker periodically to sustain TCP connection
  if (millis() - lastKeepAlive > 30000) {
    lastKeepAlive = millis();
    if (!mqtt.ping()) {
      Serial.println("[MQTT Ping] Lost connection to broker. Will reconnect...");
      mqtt.disconnect();
    }
  }
}

// -----------------------------------------------------------------------------
// Wi-Fi Connection
// -----------------------------------------------------------------------------
void connectWiFi() {
  Serial.print("[Wi-Fi] Connecting to network: ");
  Serial.println(WLAN_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n[Wi-Fi] Connected successfully!");
  Serial.print("[Wi-Fi] Local IP: ");
  Serial.println(WiFi.localIP());
}

// -----------------------------------------------------------------------------
// MQTT Connection
// -----------------------------------------------------------------------------
void connectMQTT() {
  if (mqtt.connected()) return;

  Serial.print("[MQTT] Connecting to Adafruit IO Broker... ");
  int8_t ret;
  uint8_t retries = 3;

  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("[MQTT] Retrying in 4 seconds...");
    mqtt.disconnect();
    delay(4000);
    retries--;
    if (retries == 0) {
      Serial.println("[MQTT] Reconnection attempt timed out. Continuing main loop...");
      return;
    }
  }
  Serial.println("[MQTT] Connected and ready for voice commands!");
}

