#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char* ssid = "XXXX";
const char* password = "XXXXXX";

// MQTT broker details private
const char* mqtt_broker = "XXXXXXXXX";
const int mqtt_port = 8883;
const char* mqtt_username = "XXXX";
const char* mqtt_password = "XXXXX";

// MQTT topics
const char* topic_publish_ir = "esp32/ir_sensor";
const char* topic_subscribe = "esp32/receive"; // Topic to receive messages

// IR Sensor details
const int ir_sensor_pin = 34; // GPIO pin connected to the IR sensor

// Create instances
WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);

// Variables for timing
long previous_time = 0;

// Callback function to handle incoming messages
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];//*(payload+i)
  }
  Serial.println(message);

  // Handle the message, e.g., turn on/off an LED based on message
  if (message == "ON") {
    Serial.println("Turning on LED");
    // Do something, like turning on an LED
  } else if (message == "OFF") {
    Serial.println("Turning off LED");
    // Do something, like turning off an LED
  }
}

void setupMQTT() {
  mqttClient.setServer(mqtt_broker, mqtt_port);
  mqttClient.setCallback(mqttCallback);
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
    Serial.println("Reconnecting to MQTT Broker...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (mqttClient.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT Broker.");

      // Subscribe to the control topic
      mqttClient.subscribe(topic_subscribe);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to Wi-Fi");

  // Initialize secure WiFiClient
  wifiClient.setInsecure(); // Use this only for testing, it allows connecting without a root certificate
  
  setupMQTT();

  pinMode(ir_sensor_pin, INPUT); // Set IR sensor pin as input
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  long now = millis();
  if (now - previous_time > 1000) { // Publish every 10 seconds
    previous_time = now;

    // Read the IR sensor value
    int ir_sensor_value = analogRead(ir_sensor_pin);
    int distance = map(ir_sensor_value, 0, 4095, 0, 30);
    
    // Convert the value to a string
    String ir_value_str = String(distance);
    
    // Publish the sensor value to the MQTT topic
    Serial.print("IR Sensor Value: ");
    Serial.println(ir_value_str);
    mqttClient.publish(topic_publish_ir, ir_value_str.c_str());
  }}