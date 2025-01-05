/*
   ESP32 code: LED control with delay and pressure detection for sensor 2
   Modified to show a message when pressure is back on sensor 2.
*/
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>

#define FORCE_SENSOR_1_PIN 35  // ESP32 pin GPIO33 (ADC1 channel)
#define FORCE_SENSOR_2_PIN 34  // ESP32 pin GPIO34 (ADC1 channel)
#define PRESSURE_THRESHOLD 20  // Threshold below which it's considered "no pressure"
#define BUZZER_PIN 21

// const char* ssid = "bletchley";       // Replace with your Wi-Fi SSID
// const char* password = "laptop!internet"; // Replace with your Wi-Fi password

const char *ssid = "Proximus-Home-327354";
const char *password = "b2pjy9sy7yh6a4e7";
const char *serverUrl = "http://192.168.129.57:8080/api/data";
const char *rewardServerUrl = "http://192.168.129.57:8080/api/reward";

// Belgium-specific NTP server
const char *ntpServer = "be.pool.ntp.org";

// Time zone settings for Belgium
const long gmtOffset_sec = 3600;      // GMT +1 hour
const int daylightOffset_sec = 3600;  // +1 hour for daylight saving time

const String name = "Luka";
const int points = 1;

bool sensor1Active = true;
bool sensor2Active = true;

// Function to play a sound (tone) on the buzzer
void playSound(int note, int duration) {
  tone(BUZZER_PIN, note, duration);  // Play the note for the specified duration
  delay(duration);                   // Wait for the note to finish
  noTone(BUZZER_PIN);                // Stop the tone after the duration
}

// Function to get the current time as a formatted string
String getFormattedTimestamp() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "";
  }

  char timestamp[20];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(timestamp);
}

void sendPostRequest() {
  if (WiFi.status() == WL_CONNECTED) {  // Check Wi-Fi connection
    HTTPClient http;
    http.begin(serverUrl);

    String timestamp = getFormattedTimestamp();

    // Set request headers and payload
    http.addHeader("Content-Type", "application/json");
    String payload = "{\"id\":1, \"sensor_value\": false, \"sensor\": \"back\", \"name\": \"" + name + "\", \"timestamp\": \"" + timestamp + "\"}";

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

    http.end();  // Close connection
  } else {
    Serial.println("Wi-Fi not connected.");
  }
}

void sendRewardPostRequest() {
  if (WiFi.status() == WL_CONNECTED) {  // Check Wi-Fi connection
    HTTPClient http;
    http.begin(rewardServerUrl);  // Use the reward API URL

    String timestamp = getFormattedTimestamp();

    // Set request headers and payload
    http.addHeader("Content-Type", "application/json");
    String payload = "{\"user\": \"" + name + "\", \"points\": " + String(points) + ", \"timestamp\": \"" + String(timestamp) + "\"}";

    // Send POST request
    int httpResponseCode = http.POST(payload);

    // Log response
    if (httpResponseCode > 0) {
      Serial.println("Reward POST request sent successfully.");
      Serial.print("Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.println("Error in sending reward POST request.");
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();  // Close connection
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
    // Configure NTP for Belgium time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  } else {
    Serial.println("\nFailed to connect to Wi-Fi after 10 attempts.");
    // Handle failure (e.g., go into a safe mode)
  }
}

unsigned long sensor2ReleaseTime = 0;               // Time when sensor 2 was last released
unsigned long cumulativeGoodWalkTime = 0;           // Cumulative active time
unsigned long lastSensor2ActiveTime = 0;            // Last time sensor 2 was active
const unsigned long PRESSURE_RELEASE_LIMIT = 3000;  // 3 seconds
const unsigned long REWARD_TIME_LIMIT = 5000;       // 5 seconds
bool postRequestSent = false;                       // Flag to track if post request has been sent

void loop() {
  // Read analog values from both sensors
  int sensor1Reading = analogRead(FORCE_SENSOR_1_PIN);
  int sensor2Reading = analogRead(FORCE_SENSOR_2_PIN);

  // Check Sensor 1 (logging only)
  if (sensor1Active && sensor1Reading < PRESSURE_THRESHOLD) {
    Serial.println("Sensor 1: Pressure released");
    sensor1Active = false;
  } else if (!sensor1Active && sensor1Reading >= PRESSURE_THRESHOLD) {
    Serial.println("Sensor 1: Pressure applied");
    sensor1Active = true;
  }

  // Check Sensor 2
  if (sensor2Active && sensor2Reading < PRESSURE_THRESHOLD) {
    Serial.println("Sensor 2: Pressure released");
    sensor2Active = false;
    sensor2ReleaseTime = millis();  // Record release time
    postRequestSent = false;        // Allow new post request after release period
  } else if (!sensor2Active && sensor2Reading >= PRESSURE_THRESHOLD) {
    Serial.println("Sensor 2: Pressure applied");
    sensor2Active = true;
    lastSensor2ActiveTime = millis();  // Update last active time
  }

  // Accumulate good walk time while sensor 2 is active
  if (sensor2Active) {
    cumulativeGoodWalkTime += millis() - lastSensor2ActiveTime;
    lastSensor2ActiveTime = millis();
  }

  // Check if sensor 2 has been released for 3 seconds
  if (!sensor2Active && (millis() - sensor2ReleaseTime >= PRESSURE_RELEASE_LIMIT)) {
    if (!postRequestSent) {
      sendPostRequest();  // Send post request only once
      postRequestSent = true;
    }
    playSound(262, 1000);  // Play sound
  }

  // Check if cumulative active time reaches the reward threshold
  if (cumulativeGoodWalkTime >= REWARD_TIME_LIMIT) {
    Serial.println("Reward: 1 point awarded" + name);
    sendRewardPostRequest();     // Send reward point to the API
    cumulativeGoodWalkTime = 0;  // Reset cumulative time after reward
  }

  delay(200);  // Small delay for stability
}
