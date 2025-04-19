#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi credentials
const char* ssid = "uuhhjoifff";
const char* password = "jjkjiooio";

// MQTT Broker settings
// If you're running the broker on a different machine than the Qt app,
// use that machine's IP instead
const char* mqtt_server = "xxxx.xxxx.xxxx.xxxx"; // Your computer's actual WiFi IP address // Change to your broker IP (not the ESP32's IP)
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

// Settings
const long PUBLISH_INTERVAL = 10000; // Publish sensor data every 10 seconds
unsigned long lastPublish = 0;

// MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  
  // Initialize pins
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Start with pump off
  
  // Connect to WiFi
  setupWifi();
  
  // Set up MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
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
  
  // Publish sensor data periodically
  unsigned long now = millis();
  if (now - lastPublish > PUBLISH_INTERVAL) {
    lastPublish = now;
    publishSensorData();
  }
}

void setupWifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
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
  // Read NTC thermistor (simplified)
  int rawValue = analogRead(TEMP_PIN);
  
  // Convert analog reading to temperature (this is simplified and should be calibrated)
  // Using simplified approximation - replace with proper NTC calculation for your sensor
  float voltage = rawValue * (3.3 / 4095.0);
  float resistance = 10000.0 * voltage / (3.3 - voltage); // Assuming 10K pull-up
  
  // Simplified Steinhart-Hart equation for NTC 10K
  float temperature = 1.0 / (log(resistance / 10000.0) / 3950.0 + 1.0 / 298.15) - 273.15;
  
  return temperature;
}

void publishSensorData() {
  if (!client.connected()) {
    Serial.println("Not connected to MQTT broker. Skipping data publish.");
    return;
  }

  // Read temperature from NTC (analog)
  float temperature = readNTCTemperature();
  
  // Read soil moisture
  int moistureRaw = analogRead(HUMIDITY_PIN);
  // Convert to percentage (adjust these values based on your sensor readings)
  int moistureMin = 3000; // Value in completely dry soil
  int moistureMax = 1000; // Value in water
  int moisturePercent = map(moistureRaw, moistureMin, moistureMax, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);
  
  // Read light level
  int lightRaw = analogRead(LDR_PIN);
  // Convert to percentage (adjust as needed)
  int lightPercent = map(lightRaw, 4095, 0, 0, 100);
  
  // Read battery voltage
  float batteryVoltage = analogRead(BATTERY_PIN) * (3.3 / 4095.0) * 2; // Assuming voltage divider
  int batteryPercent = map(batteryVoltage * 100, 320, 420, 0, 100); // Map 3.2V-4.2V to 0-100%
  batteryPercent = constrain(batteryPercent, 0, 100);
  
  // Convert values to strings
  char tempStr[8];
  dtostrf(temperature, 1, 2, tempStr);
  
  char humStr[8];
  sprintf(humStr, "%d", moisturePercent);
  
  char lightStr[8];
  sprintf(lightStr, "%d", lightPercent);
  
  char batteryStr[8];
  sprintf(batteryStr, "%d", batteryPercent);
  
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
  
  if (success) {
    Serial.println("% - Published successfully");
  } else {
    Serial.println("% - Publish failed!");
  }
}

void startIrrigation() {
  digitalWrite(RELAY_PIN, HIGH); // Turn on relay (and pump)
  client.publish(status_topic, "Irrigation started");
  Serial.println("Irrigation started");
}

void stopIrrigation() {
  digitalWrite(RELAY_PIN, LOW); // Turn off relay (and pump)
  client.publish(status_topic, "Irrigation stopped");
  Serial.println("Irrigation stopped");
}
