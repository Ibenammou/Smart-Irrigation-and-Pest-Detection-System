#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi credentials
const char* ssid = "hack_658";
const char* password = "mirAmaR@302+44";
// MQTT Broker settings
const char* mqtt_server = "192.168.1.13"; // Your computer's actual WiFi IP address
const int mqtt_port = 1883;
const char* mqtt_client_id = "ESP32_Irrigation";

// Topics
const char* temp_topic = "sensors/basilic/temperature";
const char* humidity_topic = "sensors/basilic/humidity";
const char* light_topic = "sensors/basilic/light";
const char* battery_topic = "sensors/basilic/battery";
const char* control_topic = "irrigation/control"; // Subscribe to this topic
const char* status_topic = "irrigation/status";   // Publish status updates

// Pin configurations
const int TEMP_PIN = 34;      // NTC temperature sensor analog pin
const int HUMIDITY_PIN = 35;  // Soil moisture sensor analog pin
const int LDR_PIN = 32;       // Light dependent resistor analog pin
const int RELAY_PIN = 26;     // Relay control pin
const int BATTERY_PIN = 33;   // Battery voltage monitoring pin

// Sensor calibration values - REVISED for better soil moisture readings
// You may need to adjust these based on your actual sensor readings
const int SOIL_DRY_RAW = 4095;    // Raw value in completely dry soil (max ADC reading)
const int SOIL_WET_RAW = 1500;    // Raw value in water (adjust based on your sensor)
const int LIGHT_MIN_RAW = 4095;   // Raw value in darkness
const int LIGHT_MAX_RAW = 0;      // Raw value in bright light
const float BATTERY_DIVIDER = 2.0; // Voltage divider ratio for battery measurement

// NTC Thermistor parameters
const float NTC_B_VALUE = 3950.0; // B constant from datasheet
const float NTC_T0 = 298.15;      // 25°C in Kelvin
const float NTC_R0 = 10000.0;     // Resistance at 25°C (10k ohm)
const float NTC_SERIES_R = 10000.0; // Series resistor (10k ohm)

// Settings
const long PUBLISH_INTERVAL = 2000; // Publish sensor data every 2 seconds (changed from 10000)
unsigned long lastPublish = 0;
bool lowPowerMode = false;          // Disable power saving (changed from true)

// Debug flag
const bool DEBUG_MODE = true;

// MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

// Variables to track irrigation state
bool irrigationActive = false;

void setup() {
  Serial.begin(115200);
  delay(1000); // Give serial monitor time to start
  
  Serial.println("\n\n----- ESP32 Irrigation System Starting -----");
  
  // Initialize pins
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Ensure pump is OFF at startup
  
  // Set analog resolution (ESP32 specific)
  analogReadResolution(12); // 12-bit resolution (0-4095)
  
  // Configure ADC attenuation for higher voltage readings
  analogSetPinAttenuation(TEMP_PIN, ADC_11db);
  analogSetPinAttenuation(HUMIDITY_PIN, ADC_11db);
  analogSetPinAttenuation(LDR_PIN, ADC_11db);
  analogSetPinAttenuation(BATTERY_PIN, ADC_11db);
  
  // Take initial readings to stabilize ADC
  for(int i=0; i<5; i++) {
    analogRead(TEMP_PIN);
    analogRead(HUMIDITY_PIN);
    analogRead(LDR_PIN);
    analogRead(BATTERY_PIN);
    delay(10);
  }
  
  Serial.println("Sensors initialized");
  Serial.println("Pump relay initialized to OFF state");
  
  // Connect to WiFi
  setupWifi();
  
  // Set up MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  // Publish initial status to confirm the pump is OFF
  if (client.connected()) {
    client.publish(status_topic, "System initialized, pump is OFF");
    Serial.println("Published initial status message");
  }
}

void loop() {
  // Check WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    setupWifi();
  }
  
  // Check MQTT connection
  if (!client.connected()) {
    reconnect();
  }
  
  // MQTT loop
  client.loop();
  
  // Publish sensor data every 2 seconds (no sleep)
  unsigned long now = millis();
  if (now - lastPublish > PUBLISH_INTERVAL) {
    lastPublish = now;
    publishSensorData();
    
    // Debug output for soil moisture raw value
    int moistureRaw = analogRead(HUMIDITY_PIN);
    Serial.print("DEBUG - Soil moisture raw value: ");
    Serial.println(moistureRaw);
  }
  
  // No deep sleep - continuous operation for real-time monitoring
}

void setupWifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("WiFi connection failed!");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Convert payload to string
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);

  // Handle irrigation control commands
  if (String(topic) == control_topic) {
    if (message == "start") {
      startIrrigation();
    } else if (message == "stop") {
      stopIrrigation();
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  int attempts = 0;
  while (!client.connected() && attempts < 3) {
    Serial.print("Attempting MQTT connection...");
    attempts++;
    
    // Debug info
    Serial.print("Broker: ");
    Serial.print(mqtt_server);
    Serial.print(":");
    Serial.print(mqtt_port);
    Serial.print(" | Client ID: ");
    Serial.println(mqtt_client_id);
    
    // Attempt to connect
    if (client.connect(mqtt_client_id)) {
      Serial.println("connected");
      
      // Subscribe to control topic
      client.subscribe(control_topic);
      
      // Publish a connection message
      client.publish(status_topic, "ESP32 irrigation system connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

float readNTCTemperature() {
  // Read and average multiple samples for more stability
  const int numSamples = 5;
  int sum = 0;
  
  for (int i = 0; i < numSamples; i++) {
    sum += analogRead(TEMP_PIN);
    delay(10);
  }
  
  int rawValue = sum / numSamples;
  
  if (DEBUG_MODE) {
    Serial.print("Raw temperature value: ");
    Serial.println(rawValue);
  }
  
  // Convert analog reading to temperature
  float voltage = rawValue * (3.3 / 4095.0);
  
  // Calculate resistance of thermistor
  float resistance;
  if (voltage > 0) {
    resistance = NTC_SERIES_R * (3.3 - voltage) / voltage;
  } else {
    // Avoid division by zero
    resistance = 999999;
  }
  
  // Steinhart-Hart equation for NTC thermistor
  float temperature;
  if (resistance > 0) {
    temperature = (NTC_B_VALUE * NTC_T0) / (NTC_B_VALUE + NTC_T0 * log(resistance / NTC_R0)) - 273.15;
  } else {
    // Error case
    temperature = -99.99;
  }
  
  // Error checking
  if (temperature < -50 || temperature > 150) {
    Serial.println("Warning: Temperature reading out of range, sensor may be disconnected");
    temperature = 25.0; // Return a reasonable default value
  }
  
  return temperature;
}

float readBatteryVoltage() {
  // Read and average multiple samples for more stability
  const int numSamples = 10;
  int sum = 0;
  
  for (int i = 0; i < numSamples; i++) {
    sum += analogRead(BATTERY_PIN);
    delay(5);
  }
  
  int rawValue = sum / numSamples;
  
  if (DEBUG_MODE) {
    Serial.print("Raw battery reading: ");
    Serial.println(rawValue);
  }
  
  // Convert to voltage based on voltage divider
  float voltage = rawValue * (3.3 / 4095.0) * BATTERY_DIVIDER;
  
  // Error check
  if (voltage < 2.5 || voltage > 4.5) {
    Serial.println("Warning: Battery voltage out of range, check voltage divider");
    voltage = 3.7; // Default to nominal LiPo voltage
  }
  
  return voltage;
}

int readSoilMoisture() {
  // Read and average multiple samples for more stability
  const int numSamples = 10; // Increased from 5 to 10 for better accuracy
  int sum = 0;
  
  for (int i = 0; i < numSamples; i++) {
    sum += analogRead(HUMIDITY_PIN);
    delay(10);
  }
  
  int moistureRaw = sum / numSamples;
  
  if (DEBUG_MODE) {
    Serial.print("Raw soil moisture value: ");
    Serial.println(moistureRaw);
  }
  
  // Convert to percentage (with more robust checking)
  int moisturePercent;
  if (SOIL_DRY_RAW != SOIL_WET_RAW) {
    moisturePercent = map(moistureRaw, SOIL_DRY_RAW, SOIL_WET_RAW, 0, 100);
    moisturePercent = constrain(moisturePercent, 0, 100);
  } else {
    moisturePercent = 50; // Default value if calibration values are incorrect
    Serial.println("Warning: Soil moisture calibration values are identical");
  }
  
  return moisturePercent;
}

void publishSensorData() {
  if (!client.connected()) {
    Serial.println("Not connected to MQTT broker. Skipping data publish.");
    return;
  }

  // Read temperature from NTC (analog)
  float temperature = readNTCTemperature();
  
  // Read soil moisture with improved function
  int moisturePercent = readSoilMoisture();
  
  // Read light level with averaging
  int lightSum = 0;
  for (int i = 0; i < 5; i++) {
    lightSum += analogRead(LDR_PIN);
    delay(10);
  }
  int lightRaw = lightSum / 5;
  
  if (DEBUG_MODE) {
    Serial.print("Raw light value: ");
    Serial.println(lightRaw);
  }
  
  // Convert to percentage (with more robust checking)
  int lightPercent;
  if (LIGHT_MIN_RAW != LIGHT_MAX_RAW) {
    lightPercent = map(lightRaw, LIGHT_MIN_RAW, LIGHT_MAX_RAW, 0, 100);
    lightPercent = constrain(lightPercent, 0, 100);
  } else {
    lightPercent = 50; // Default value if calibration values are incorrect
    Serial.println("Warning: Light calibration values are identical");
  }
  
  // Read battery voltage with better calculation
  float batteryVoltage = readBatteryVoltage();
  
  // Map battery percentage based on LiPo battery characteristics
  // Typical LiPo battery: 3.2V (empty) to 4.2V (full)
  int batteryPercent = map(int(batteryVoltage * 100), 320, 420, 0, 100);
  batteryPercent = constrain(batteryPercent, 0, 100);
  
  // Convert values to strings with appropriate precision
  char tempStr[8];
  dtostrf(temperature, 1, 2, tempStr);
  
  char humStr[8];
  sprintf(humStr, "%d", moisturePercent);
  
  char lightStr[8];
  sprintf(lightStr, "%d", lightPercent);
  
  char batteryStr[8];
  dtostrf(batteryVoltage, 1, 2, batteryStr);
  
  // Publish to MQTT topics
  bool success = true;
  success &= client.publish(temp_topic, tempStr);
  success &= client.publish(humidity_topic, humStr);
  success &= client.publish(light_topic, lightStr);
  success &= client.publish(battery_topic, batteryStr);
  
  // Debug output
  Serial.print("Temperature: ");
  Serial.print(tempStr);
  Serial.print("°C, Humidity: ");
  Serial.print(humStr);
  Serial.print("%, Light: ");
  Serial.print(lightStr);
  Serial.print("%, Battery: ");
  Serial.print(batteryStr);
  Serial.print("V");
  
  if (success) {
    Serial.println(" - Published successfully");
  } else {
    Serial.println(" - Publish failed!");
  }
}

void startIrrigation() {
  digitalWrite(RELAY_PIN, HIGH); // Turn on relay (assuming active-high)
  irrigationActive = true;
  client.publish(status_topic, "Irrigation started");
  Serial.println("Irrigation started");
}

void stopIrrigation() {
  digitalWrite(RELAY_PIN, LOW); // Turn off relay
  irrigationActive = false;
  client.publish(status_topic, "Irrigation stopped");
  Serial.println("Irrigation stopped");
}
