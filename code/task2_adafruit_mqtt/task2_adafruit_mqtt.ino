/*
  ==============================================================================
  Project: Week 7 - Task 2: Adafruit IO Dashboard & MQTT Cloud Control
  Author: Archana C
  Institution: Kumaraguru College of Technology (KCT)
  Repository: https://github.com/archin123/portfolio
  Target Board: ESP32 Dev Module
  Description:
    Connects ESP32 to the Adafruit IO Cloud via MQTT protocol (Port 1883/8883).
    Subscribes to feed 'archana_07/feeds/bulb'. When a user toggles the switch
    on the Adafruit IO dashboard, the MQTT broker distributes the payload to
    the ESP32, which actuates a 5V SPDT Relay module controlling a 230V bulb.
  ==============================================================================
*/

#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// -----------------------------------------------------------------------------
// Network & Adafruit IO Credentials
// -----------------------------------------------------------------------------
#define WLAN_SSID       "YOUR_WIFI_SSID"
#define WLAN_PASS       "YOUR_WIFI_PASSWORD"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // 1883 for unencrypted MQTT, 8883 for SSL/TLS
#define AIO_USERNAME    "archana_07"           // Adafruit IO Account Username
#define AIO_KEY         "aio_YOUR_SECRET_AIO_KEY" // Keep private in deployment

// -----------------------------------------------------------------------------
// Hardware Pin Definitions
// -----------------------------------------------------------------------------
// Relay control input connected to GPIO 26
const int RELAY_PIN = 26;

// Relay module logic configuration (Most 5V relay modules are active-LOW)
const bool RELAY_ACTIVE_STATE = LOW;   // LOW turns relay coil ON
const bool RELAY_INACTIVE_STATE = HIGH; // HIGH turns relay coil OFF

// -----------------------------------------------------------------------------
// MQTT Client & Feed Subscription Setup
// -----------------------------------------------------------------------------
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Setup a feed called 'bulb' for subscribing to changes:
// Full MQTT Topic: "archana_07/feeds/bulb"
Adafruit_MQTT_Subscribe bulbFeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/bulb");

// -----------------------------------------------------------------------------
// Function Prototypes
// -----------------------------------------------------------------------------
void MQTT_connect();

// -----------------------------------------------------------------------------
// Callback Function for Incoming MQTT Messages
// -----------------------------------------------------------------------------
void bulbCallback(char *data, uint16_t len) {
  String command = String(data);
  command.trim();

  Serial.print("Received command: ");
  Serial.println(command);

  if (command.equalsIgnoreCase("ON") || command == "1") {
    digitalWrite(RELAY_PIN, RELAY_ACTIVE_STATE);
    Serial.println("Relay: ON");
    Serial.println("Bulb: ON");
  } else if (command.equalsIgnoreCase("OFF") || command == "0") {
    digitalWrite(RELAY_PIN, RELAY_INACTIVE_STATE);
    Serial.println("Relay: OFF");
    Serial.println("Bulb: OFF");
  } else {
    Serial.print("Unknown command payload: ");
    Serial.println(command);
  }
}

// -----------------------------------------------------------------------------
// Setup Function
// -----------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n==========================================");
  Serial.println("  Adafruit IO Dashboard & MQTT Control    ");
  Serial.println("  Student: Archana C (Account: archana_07)");
  Serial.println("==========================================");

  // Initialize Relay control pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_INACTIVE_STATE); // Default to bulb OFF
  Serial.println("Bulb: OFF");

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi SSID: ");
  Serial.println(WLAN_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Attach callback function to the bulb feed subscription
  bulbFeed.setCallback(bulbCallback);
  mqtt.subscribe(&bulbFeed);

  Serial.println("Subscribed to MQTT Feed: " AIO_USERNAME "/feeds/bulb");
}

// -----------------------------------------------------------------------------
// Main Loop
// -----------------------------------------------------------------------------
void loop() {
  // Ensure the connection to the MQTT server is alive
  MQTT_connect();

  // Process incoming subscription packets and trigger registered callbacks
  // Timeout set to 5000 ms to yield processor time gracefully
  mqtt.processPackets(5000);

  // Send keepalive ping to prevent MQTT broker disconnection timeout
  if (!mqtt.ping()) {
    mqtt.disconnect();
  }
}

// -----------------------------------------------------------------------------
// Robust MQTT Connection Handler with Exponential Backoff
// -----------------------------------------------------------------------------
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to Adafruit IO MQTT Broker... ");

  uint8_t retries = 5;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for success
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying Adafruit IO connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0) {
      Serial.println("MQTT Connection failed. Resetting ESP32 watchdog...");
      // Wait for next loop iteration
      return;
    }
  }
  Serial.println("Adafruit IO Connected Successfully!");
}

