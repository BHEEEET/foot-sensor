/*
   ESP32 code: LED control with delay and pressure detection for sensor 2
   Modified to show a message when pressure is back on sensor 2.
*/
#include <WiFi.h>
#include <HTTPClient.h>

#define FORCE_SENSOR_1_PIN 34 // ESP32 pin GPIO33 (ADC1 channel)
#define FORCE_SENSOR_2_PIN 35 // ESP32 pin GPIO34 (ADC1 channel)
#define PRESSURE_THRESHOLD 20 // Threshold below which it's considered "no pressure"
#define BUZZER_PIN 21

// const char* ssid = "bletchley";       // Replace with your Wi-Fi SSID
// const char* password = "laptop!internet"; // Replace with your Wi-Fi password


// NIET VERGETEN DE IP ADDRESS TE VERANDEREN
const char* serverUrl = "http://192.168.0.20:8080/api/data"; // Replace with your API endpoint

bool sensor1Active = true; // Tracks if Sensor 1 is currently pressed
bool sensor2Active = true; // Tracks if Sensor 2 is currently pressed


// Function to play a sound (tone) on the buzzer
void playSound(int note, int duration) {
  tone(BUZZER_PIN, note, duration);  // Play the note for the specified duration
  delay(duration);                  // Wait for the note to finish
  noTone(BUZZER_PIN);                // Stop the tone after the duration
}

void sendPostRequest() {
  if (WiFi.status() == WL_CONNECTED) { // Check Wi-Fi connection
    HTTPClient http;
    http.begin(serverUrl);
        
    // Get the timestamp using millis()
    unsigned long timestamp = millis();

    // Set request headers and payload
    http.addHeader("Content-Type", "application/json");
    String payload = "{\"id\":1, \"sensor_value\": false, \"sensor\": \"back\"}";

    
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

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);  // Ensure the buzzer pin is set to output

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  int attempts = 0;
  
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {  // Retry up to 10 times
    delay(1000);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to Wi-Fi!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to Wi-Fi after 10 attempts.");
    // Handle failure (e.g., go into a safe mode)
  }
}


unsigned long sensor2ReleaseTime = 0;  // Time when sensor 2 was last released
const unsigned long PRESSURE_RELEASE_TIME = 3000; // 3 seconds
bool postRequestSent = false; // Flag to track if post request has been sent

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

  // Check Sensor 2 
  if (sensor2Active && sensor2Reading < PRESSURE_THRESHOLD) {
    Serial.println("Sensor 2: Pressure released");
    sensor2Active = false; // Update state to "inactive"
    sensor2ReleaseTime = millis();  // Start counting release time
  } else if (!sensor2Active && sensor2Reading >= PRESSURE_THRESHOLD) {
    Serial.println(sensor2Reading);
    Serial.println("Sensor 2: Pressure applied");
    sensor2Active = true; // Update state to "active"
    postRequestSent = false;
    
    // Reset timer when pressure is applied again
    // if (millis() - sensor2ReleaseTime >= PRESSURE_RELEASE_TIME) {
    //   // If pressure was released for 3 seconds, perform actions
    //   sendPostRequest();
    //   playSound(262, 1000);
    // }
  }

  // Check if sensor 2 has been released for 3 seconds
  if (!sensor2Active && millis() - sensor2ReleaseTime >= PRESSURE_RELEASE_TIME) {
    // If 3 seconds have passed since release, perform actions
    if (!postRequestSent) {
      sendPostRequest(); // Send post request only once
      postRequestSent = true; // Set the flag to true to prevent sending again
    }
    playSound(262, 1000); // Play sound every time the pressure is released for 3 seconds
  }

  delay(200); // Small delay for stability
}

