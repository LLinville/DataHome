#include "config.h"
#include <DHT_U.h>
#include <DHT.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#define DHTPIN 2

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

#define REST_PORT 60000

ESP8266WebServer server(REST_PORT);

// Replace with your network credentials
const char* ssid     = WIFI_SSID;
const char* password = WIFI_PASSWORD;
String boardId = "1";
String sensorId = "1";

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

// Manage not found URL
void handleNotFound() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void serverRouting() {
    server.on("/", HTTP_GET, []() {
        String message = "Sensor rest server for board " + boardId;
        server.send(200, F("text/html"), message);
    });
    server.on(F("/sensor/info"), HTTP_GET, getHealthCheck);
}

void getHealthCheck() {
    String currentTime = String(millis());
    //delay(2500);
    float h = dht.readHumidity();
    //delay(2500);
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    //delay(2500);
    String json = "{\n";
    json += "\"boardUptime\":" + currentTime;
    json += ",\n\"boardId\":" + boardId;
    json += ",\n\"sensorId\":" + sensorId;
    json += ",\n\"temperatureC\":" + String(t);
    json += ",\n\"humidity\":" + String(h);
    json +="\n}";
    server.send(200, F("text/plain"), json);
}

void getSensorDataSerial() {
    delay(2500);
    float h = dht.readHumidity(true);
    delay(2500);
    Serial.println(h);
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature(false, true);
    delay(2500);
    Serial.println(t);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
}

void setup() {

    Serial.begin(115200);

    // Connect to Wi-Fi network with SSID and password
    Serial.println("Board ID: " + boardId);
    Serial.println("Sensor ID: " + sensorId);
    Serial.println("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("Board ID: " + boardId);
    Serial.println("WiFi connected to " + WiFi.SSID());
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    serverRouting();
    server.onNotFound(handleNotFound);
    server.begin();

    dht.begin();
}

void loop(){
    server.handleClient();
}