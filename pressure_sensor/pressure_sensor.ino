/*
   ESP32 code: LED control with delay and pressure detection for sensor 2
   Modified to show a message when pressure is back on sensor 2.
*/
#include <WiFi.h>
#include <HTTPClient.h>

#define FORCE_SENSOR_1_PIN 33 // ESP32 pin GPIO33 (ADC1 channel)
#define FORCE_SENSOR_2_PIN 34 // ESP32 pin GPIO34 (ADC1 channel)
#define LED_PIN 19           // ESP32 pin GPIO32 for the LED
#define PRESSURE_THRESHOLD 50 // Threshold below which it's considered "no pressure"
#define LED_OFF_DELAY 2000    // Delay in milliseconds to keep the LED off

const char* ssid = "bletchley";       // Replace with your Wi-Fi SSID
const char* password = "laptop!internet"; // Replace with your Wi-Fi password
const char* serverUrl = "http://10.150.200.129:8080/test"; // Replace with your API endpoint

bool sensor1Active = true; // Tracks if Sensor 1 is currently pressed
bool sensor2Active = true; // Tracks if Sensor 2 is currently pressed

void sendPostRequest() {
  if (WiFi.status() == WL_CONNECTED) { // Check Wi-Fi connection
    HTTPClient http;
    http.begin(serverUrl);

    // Set request headers and payload
    http.addHeader("Content-Type", "application/json");
    String payload = "{\"sensor\":\"sensor2\", \"status\":\"released\"}";
    
    // Send POST request
    int httpResponseCode = http.POST(payload);

    // Log response
    if (httpResponseCode > 0) {
      Serial.println("POST request sent successfully.");
      Serial.print("Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.println("Error in sending POST request.");
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end(); // Close connection
  } else {
    Serial.println("Wi-Fi not connected.");
  }
}

void controlLED(bool state) {
  if (state) {
    digitalWrite(LED_PIN, HIGH); // Turn on the LED
    delay(LED_OFF_DELAY);        // Wait before allowing it to turn back on
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn off the LED
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT); // Set the LED pin as an output

    // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Read analog values from both sensors
  int sensor1Reading = analogRead(FORCE_SENSOR_1_PIN);
  int sensor2Reading = analogRead(FORCE_SENSOR_2_PIN);

  // Check Sensor 1 (no LED control, just logging)
  if (sensor1Active && sensor1Reading < PRESSURE_THRESHOLD) {
    Serial.println("Sensor 1: Pressure released");
    sensor1Active = false; // Update state to "inactive"
  } else if (!sensor1Active && sensor1Reading >= PRESSURE_THRESHOLD) {
    Serial.println("Sensor 1: Pressure applied");
    sensor1Active = true; // Update state to "active"
  }

  // Check Sensor 2 and control the LED
  if (sensor2Active && sensor2Reading < PRESSURE_THRESHOLD) {
    Serial.println("Sensor 2: Pressure released");
    sensor2Active = false; // Update state to "inactive"
    sendPostRequest();
    controlLED(true); // Turn on the LED
  } else if (!sensor2Active && sensor2Reading >= PRESSURE_THRESHOLD) {
    Serial.println("Sensor 2: Pressure applied");
    sensor2Active = true; // Update state to "active"
    controlLED(false); // Turn off the LED with a delay
  }

  delay(100); // Small delay for stability
}
