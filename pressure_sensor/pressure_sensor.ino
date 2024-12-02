/*
   This ESP32 code is created by esp32io.com
   Modified to detect only when the pressure is released
   This ESP32 code is released in the public domain
*/

#define FORCE_SENSOR_1_PIN 33 // ESP32 pin GPIO33 (ADC1 channel)
#define FORCE_SENSOR_2_PIN 34 // ESP32 pin GPIO34 (ADC1 channel)
#define PRESSURE_THRESHOLD 50 // Threshold below which it's considered "no pressure"

bool sensor1Active = true; // Tracks if Sensor 1 is currently pressed
bool sensor2Active = true; // Tracks if Sensor 2 is currently pressed

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Read analog values from both sensors
  int sensor1Reading = analogRead(FORCE_SENSOR_1_PIN);
  int sensor2Reading = analogRead(FORCE_SENSOR_2_PIN);

  // Check Sensor 1
  if (sensor1Active && sensor1Reading < PRESSURE_THRESHOLD) {
    Serial.println("Sensor Front: Pressure released");
    sensor1Active = false; // Update state to "inactive"
  } else if (!sensor1Active && sensor1Reading >= PRESSURE_THRESHOLD) {
    sensor1Active = true; // Update state to "active" when pressure is reapplied
  }

  // Check Sensor 2
  if (sensor2Active && sensor2Reading < PRESSURE_THRESHOLD) {
    Serial.println("Sensor Back: Pressure released");
    sensor2Active = false; // Update state to "inactive"
  } else if (!sensor2Active && sensor2Reading >= PRESSURE_THRESHOLD) {
    sensor2Active = true; // Update state to "active" when pressure is reapplied
  }

  delay(100); // Small delay for stability
}
