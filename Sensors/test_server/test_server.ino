#include "config.h"
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>


#define REST_PORT 60000

ESP8266WebServer server(REST_PORT);

// Replace with your network credentials
const char* ssid     = WIFI_SSID;
const char* password = WIFI_PASSWORD;
String boardId = "3";
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
        server.send(200, F("text/html"),
            F("Welcome to the REST Web Server"));
    });
    server.on(F("/health/info"), HTTP_GET, getHealthCheck);
}

void getHealthCheck() {
    String currentTime = String(millis());
    String json = "{\"currentTime\":" + currentTime;
    json += ",\n\"boardId\":" + boardId;
    json += ",\n\"sensorId\":" + sensorId;
    json +="}";
    server.send(200, F("text/plain"), json);
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
}

void loop(){
    server.handleClient();
}