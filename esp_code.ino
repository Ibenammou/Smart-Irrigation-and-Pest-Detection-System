#include <WiFi.h>
#include <HTTPClient.h>

#define SOIL_MOISTURE_PIN 34
#define TEMP_SENSOR_PIN 35
#define WATER_PUMP_PIN 5
#define CAMERA_MODULE_PIN 13

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* serverURL = "http://your-server.com/data";

void setup() {
    Serial.begin(115200);
    pinMode(SOIL_MOISTURE_PIN, INPUT);
    pinMode(TEMP_SENSOR_PIN, INPUT);
    pinMode(WATER_PUMP_PIN, OUTPUT);
    pinMode(CAMERA_MODULE_PIN, INPUT);

    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to WiFi");
}

void loop() {
    int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
    int temperature = analogRead(TEMP_SENSOR_PIN); 
    bool pestDetected = digitalRead(CAMERA_MODULE_PIN);

    Serial.print("Soil Moisture: ");
    Serial.println(soilMoisture);
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Pest Detected: ");
    Serial.println(pestDetected);

    if (soilMoisture < 500) {
        digitalWrite(WATER_PUMP_PIN, HIGH);
        Serial.println("Water Pump ON");
    } else {
        digitalWrite(WATER_PUMP_PIN, LOW);
        Serial.println("Water Pump OFF");
    }

    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverURL);
        http.addHeader("Content-Type", "application/json");

        String jsonPayload = "{"soilMoisture": " + String(soilMoisture) + ", "temperature": " + String(temperature) + ", "pestDetected": " + String(pestDetected) + "}";
        int httpResponseCode = http.POST(jsonPayload);

        if (httpResponseCode > 0) {
            Serial.print("Data sent successfully: ");
            Serial.println(httpResponseCode);
        } else {
            Serial.print("Error sending data: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    }

    delay(5000);
}

