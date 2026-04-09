#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>

// WiFi and backend settings
const char* WIFI_SSID = "Nord 3";
const char* WIFI_PASSWORD = "daksh0708";
const char* SERVER_URL = "http://10.100.70.36/parking/android_db_pool/Sensor_data.php";

// ESP32 pins
const int SENSOR1_PIN = 32; // Slot 1 sensor
const int SENSOR2_PIN = 33; // Slot 2 sensor
const int SENSOR3_PIN = 34; // Slot 3 sensor (input-only pin)
const int SERVO_PIN = 18;   // Gate servo signal

// Servo angles
const int SERVO_CLOSED_ANGLE = 0;
const int SERVO_OPEN_ANGLE = 90;

Servo gateServo;
unsigned long lastUploadMs = 0;
const unsigned long UPLOAD_INTERVAL_MS = 2000;

String buildSensorLabel(bool isOccupied, const String& onLabel, const String& offLabel) {
  return isOccupied ? onLabel : offLabel;
}

void operateGate() {
  gateServo.write(SERVO_OPEN_ANGLE);
  delay(5000);
  gateServo.write(SERVO_CLOSED_ANGLE);
  delay(1000);
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected. ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);

  pinMode(SENSOR1_PIN, INPUT);
  pinMode(SENSOR2_PIN, INPUT);
  pinMode(SENSOR3_PIN, INPUT);

  gateServo.attach(SERVO_PIN);
  gateServo.write(SERVO_CLOSED_ANGLE);

  connectWiFi();
}

void loop() {
  if (millis() - lastUploadMs < UPLOAD_INTERVAL_MS) {
    return;
  }
  lastUploadMs = millis();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Reconnecting...");
    connectWiFi();
    return;
  }

  // Project's original logic: LOW means car present (on), HIGH means empty (off)
  String sensor1 = buildSensorLabel(digitalRead(SENSOR1_PIN) == LOW, "p1s1on", "p1s1off");
  String sensor2 = buildSensorLabel(digitalRead(SENSOR2_PIN) == LOW, "p1s2on", "p1s2off");
  String sensor3 = buildSensorLabel(digitalRead(SENSOR3_PIN) == LOW, "p1s3on", "p1s3off");

  String postBody = "sensor1=" + sensor1 + "&sensor2=" + sensor2 + "&sensor3=" + sensor3;

  HTTPClient http;
  http.begin(SERVER_URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int code = http.POST(postBody);
  if (code > 0) {
    String response = http.getString();
    response.trim();

    Serial.print("POST code: ");
    Serial.print(code);
    Serial.print(" | response: ");
    Serial.println(response);

    if (response == "1" || response == "2" || response == "3") {
      operateGate();
    }
  } else {
    Serial.print("HTTP POST failed. Error code: ");
    Serial.println(code);
  }

  http.end();
}
