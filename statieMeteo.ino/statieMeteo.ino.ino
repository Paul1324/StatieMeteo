#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

// Replace with your Wi-Fi credentials
const char* ssid = "2.4GG";
const char* password = "159357M?";

// DHT sensor pin
const int dhtPin = 2; // GPIO4 for DHT sensor
const int optPin = 4; // GPIO5 for optical sensor

// DHT sensor
DHT dht(dhtPin, DHT11);

// Counter for rotations
volatile unsigned long rotations = 0;

// Time variables
unsigned long previousMillis = 0;
const unsigned long interval = 1000; // Interval in milliseconds

// Wind speed conversion factor
const float windSpeedConversion = 1.0; // Adjust this value based on your specific setup

// Create a web server
ESP8266WebServer server(80);
// Interrupt handler function
void ICACHE_RAM_ATTR countRotation() {
  rotations++;
}
void setup() {
  Serial.begin(115200);
  dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up the optical sensor as an interrupt input
  pinMode(optPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(optPin), countRotation, RISING);

  // Set up the web server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  // Calculate wind speed every 'interval' milliseconds
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Disable interrupts temporarily to avoid conflicts
    detachInterrupt(digitalPinToInterrupt(optPin));

    // Calculate wind speed
    float windSpeed = (rotations / (interval / 1000.0)) * windSpeedConversion;

    // Reset the rotations counter
    rotations = 0;

    // Re-enable interrupts
    attachInterrupt(digitalPinToInterrupt(optPin), countRotation, RISING);

    // Print wind speed to the serial monitor
    Serial.print("Wind speed: ");
    Serial.print(windSpeed);
    Serial.println(" km/h");
  }
}

void handleRoot() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Calculate wind speed
  float windSpeed = (rotations / millis() - previousMillis) * windSpeedConversion;

  String jsonData = "{";
  jsonData += "\"temperature\":" + String(temperature) + ",";
  jsonData += "\"humidity\":" + String(humidity) + ",";
  jsonData += "\"windSpeed\":" + String(windSpeed);
  jsonData += "}";

  server.send(200, "application/json", jsonData);
}

