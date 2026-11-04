/*
  ==============================================================================
  Project: Week 7 - Task 1: ESP32 Web Server & HTML LED Control
  Author: Archana C
  Institution: Kumaraguru College of Technology (KCT)
  Repository: https://github.com/archin123/portfolio
  Target Board: ESP32 Dev Module / NodeMCU-32S
  Description:
    Hosts an asynchronous HTTP web server on an ESP32 in Station (STA) mode.
    Serves a responsive HTML/CSS web interface allowing users on the same Wi-Fi
    network to control an LED (built-in GPIO2 or external GPIO4/GPIO26) via
    HTTP GET requests (/on, /off, /status).
  ==============================================================================
*/

#include <WiFi.h>
#include <WebServer.h>

// -----------------------------------------------------------------------------
// Network Configuration (Replace with local 2.4 GHz credentials)
// -----------------------------------------------------------------------------
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// -----------------------------------------------------------------------------
// Hardware Pin Configuration
// -----------------------------------------------------------------------------
// GPIO 2 is commonly connected to the onboard blue LED on ESP32 DevKit boards.
// If using an external LED with a 220 Ohm current-limiting resistor, connect
// Anode to GPIO 2 (or GPIO 4) and Cathode through 220 Ohm resistor to GND.
const int LED_PIN = 2;

// Track active LED state
bool ledState = false;

// Create WebServer instance listening on standard HTTP port 80
WebServer server(80);

// -----------------------------------------------------------------------------
// HTML & CSS Webpage Generation Function
// Matches the modern dark-themed web interface captured in DevTools (task one 2.png)
// -----------------------------------------------------------------------------
String generateWebpage() {
  String page = "<!DOCTYPE html>\n";
  page += "<html lang=\"en\">\n";
  page += "<head>\n";
  page += "  <meta charset=\"UTF-8\">\n";
  page += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  page += "  <title>ESP32 LED Control - Wi-Fi Web Server</title>\n";
  page += "  <style>\n";
  page += "    * { margin:0; padding:0; box-sizing:border-box; font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif; }\n";
  page += "    body { background-color: #1e2430; min-height: 100vh; display: flex; align-items: center; justify-content: center; padding: 20px; }\n";
  page += "    .card { background: #2b3445; border-radius: 24px; padding: 40px 32px; width: 100%; max-width: 360px; text-align: center; box-shadow: 0 20px 40px rgba(0,0,0,0.35); }\n";
  page += "    h1 { color: #ffffff; font-size: 24px; font-weight: 700; margin-bottom: 6px; }\n";
  page += "    .subtitle { color: #8c9ba5; font-size: 14px; margin-bottom: 30px; }\n";
  page += "    .led-indicator { width: 90px; height: 90px; border-radius: 50%; margin: 0 auto 24px; transition: all 0.3s ease; ";
  if (ledState) {
    page += "background: #22c55e; box-shadow: 0 0 32px #22c55e, inset 0 0 10px #ffffff; }\n";
  } else {
    page += "background: #374151; box-shadow: inset 0 3px 8px rgba(0,0,0,0.6); }\n";
  }
  page += "    .status-text { color: #e2e8f0; font-size: 18px; font-weight: 600; margin-bottom: 32px; }\n";
  page += "    .status-text span { color: " + String(ledState ? "#22c55e" : "#ef4444") + "; font-weight: 700; }\n";
  page += "    .btn-group { display: flex; gap: 14px; justify-content: center; }\n";
  page += "    .btn { flex: 1; padding: 14px 20px; border-radius: 12px; font-size: 16px; font-weight: 700; text-decoration: none; border: none; cursor: pointer; transition: transform 0.15s ease, box-shadow 0.15s ease; display: inline-block; }\n";
  page += "    .btn:active { transform: scale(0.96); }\n";
  page += "    .btn-on { background: #22c55e; color: #ffffff; box-shadow: 0 4px 14px rgba(34,197,94,0.4); }\n";
  page += "    .btn-on:hover { background: #16a34a; }\n";
  page += "    .btn-off { background: #ef4444; color: #ffffff; box-shadow: 0 4px 14px rgba(239,68,68,0.4); }\n";
  page += "    .btn-off:hover { background: #dc2626; }\n";
  page += "    .footer { margin-top: 28px; font-size: 11px; color: #64748b; letter-spacing: 0.05em; text-transform: uppercase; }\n";
  page += "  </style>\n";
  page += "</head>\n";
  page += "<body>\n";
  page += "  <div class=\"card\">\n";
  page += "    <h1>ESP32 LED Control</h1>\n";
  page += "    <div class=\"subtitle\">Wi-Fi Web Server</div>\n";
  page += "    <div class=\"led-indicator\"></div>\n";
  page += "    <div class=\"status-text\">LED Status: <span>" + String(ledState ? "ON" : "OFF") + "</span></div>\n";
  page += "    <div class=\"btn-group\">\n";
  page += "      <a href=\"/on\" class=\"btn btn-on\">ON</a>\n";
  page += "      <a href=\"/off\" class=\"btn btn-off\">OFF</a>\n";
  page += "    </div>\n";
  page += "    <div class=\"footer\">ESP32 IP: " + WiFi.localIP().toString() + "</div>\n";
  page += "  </div>\n";
  page += "</body>\n";
  page += "</html>\n";
  return page;
}

// -----------------------------------------------------------------------------
// HTTP Route Handlers
// -----------------------------------------------------------------------------

// Handler for root path: GET /
void handleRoot() {
  Serial.println("[HTTP] GET / received - Serving main control page");
  server.send(200, "text/html", generateWebpage());
}

// Handler for turning LED ON: GET /on
void handleLEDOn() {
  ledState = true;
  digitalWrite(LED_PIN, HIGH);
  Serial.println("[HTTP] GET /on received -> LED set to HIGH (ON)");
  // HTTP 200 response with updated HTML interface
  server.send(200, "text/html", generateWebpage());
}

// Handler for turning LED OFF: GET /off
void handleLEDOff() {
  ledState = false;
  digitalWrite(LED_PIN, LOW);
  Serial.println("[HTTP] GET /off received -> LED set to LOW (OFF)");
  // HTTP 200 response with updated HTML interface
  server.send(200, "text/html", generateWebpage());
}

// Optional JSON status endpoint for automated testing / API calls
void handleStatus() {
  String json = "{\"status\": \"" + String(ledState ? "ON" : "OFF") + "\", \"gpio\": " + String(LED_PIN) + ", \"ip\": \"" + WiFi.localIP().toString() + "\"}";
  server.send(200, "application/json", json);
}

// Handler for unknown routes: 404 Not Found
void handleNotFound() {
  String message = "404 Not Found\n\n";
  message += "URI: " + server.uri() + "\n";
  message += "Method: " + String((server.method() == HTTP_GET) ? "GET" : "POST") + "\n";
  server.send(404, "text/plain", message);
}

// -----------------------------------------------------------------------------
// Setup Function
// -----------------------------------------------------------------------------
void setup() {
  // Initialize Serial Monitor at 115200 baud
  Serial.begin(115200);
  delay(500);

  Serial.println("\n==========================================");
  Serial.println("  ESP32 Web Server & HTML LED Control     ");
  Serial.println("  Student: Archana C                      ");
  Serial.println("==========================================");

  // Configure LED GPIO as output and set initial state to OFF
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  ledState = false;

  // Connect to the local Wi-Fi network in Station (STA) mode
  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait until connection is successfully established
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print network diagnostics to Serial Monitor
  Serial.println("\nWi-Fi Connected!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal Strength (RSSI): ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");

  // Register HTTP endpoint routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/on", HTTP_GET, handleLEDOn);
  server.on("/off", HTTP_GET, handleLEDOff);
  server.on("/status", HTTP_GET, handleStatus);
  server.onNotFound(handleNotFound);

  // Start HTTP Web Server
  server.begin();
  Serial.println("Web Server Started! Listening on port 80.");
  Serial.println("Open http://" + WiFi.localIP().toString() + " in your browser.");
  Serial.println("------------------------------------------");
}

// -----------------------------------------------------------------------------
// Main Loop
// -----------------------------------------------------------------------------
void loop() {
  // Continuously listen for incoming client HTTP connections and dispatch handlers
  server.handleClient();
  
  // Minimal yield to keep ESP32 watchdog and background Wi-Fi tasks healthy
  delay(2);
}

