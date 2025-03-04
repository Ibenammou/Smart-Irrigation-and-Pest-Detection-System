#include <WiFi.h>
#include <PubSubClient.h>

// Replace with your MQTT Broker IP address
const char* mqtt_server = "mqtt.eclipse.org"; // ubuntu
WiFiClient espClient;
PubSubClient client(espClient);

// Sensor pins
const int soilMoisturePin = 34;
const int temperaturePin = 35;
const int luxPin = 32;

// Relay pin
const int relayPin = 25;

// Wi-Fi credentials (replace with your own)
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++) {
        messageTemp += (char)message[i];
    }
    Serial.println(messageTemp);

    if (String(topic) == "irrigation/control") {
        if (messageTemp == "ON") {
            digitalWrite(relayPin, HIGH);
            Serial.println("Irrigation ON");
        } else if (messageTemp == "OFF") {
            digitalWrite(relayPin, LOW);
            Serial.println("Irrigation OFF");
        }
    }
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP32Client")) {
            Serial.println("connected");
            client.subscribe("irrigation/control");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void setup() {
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW);

    Serial.begin(115200);
    setup_wifi(); // Connect to Wi-Fi
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    // Read sensor data
    int soilMoisture = analogRead(soilMoisturePin);
    int temperature = analogRead(temperaturePin);
    int lux = analogRead(luxPin);

    // Publish sensor data
    String sensorData = "Soil Moisture: " + String(soilMoisture) + ", Temperature: " + String(temperature) + ", Lux: " + String(lux);
    client.publish("sensor/data", sensorData.c_str());

    delay(5000); // Wait for 5 seconds before next reading
}
