/*
 * =====================================================================
 *  Smart Parking System — ESP32 Firmware
 *  Verified against: Dakshbir/Smart-Parking-System-IoT source PHP files
 *
 *  DB: car_parking  |  Tables: data, motor, booking_up
 *
 *  Full verified API flow:
 *
 *  [1] ESP32 → POST /parking/android_db_pool/Sensor_data.php
 *        form-urlencoded body:
 *          sensor1 = p1s1on | p1s1off   ← IR_PIN_A (entry gate)
 *          sensor2 = p1s2on | p1s2off   ← IR_PIN_B (slot 1)
 *          sensor3 = p1s3on | p1s3off   ← IR_PIN_C (slot 2)
 *
 *        PHP does:
 *          INSERT INTO data (Sensor1, Sensor2, Sensor3)
 *            VALUES (sensor3_val, sensor2_val, sensor1_val)
 *            ← NOTE: original PHP has sensor1/sensor3 swapped in INSERT,
 *               but this is a bug in the repo. Does NOT affect gate logic.
 *          SELECT slot FROM motor WHERE no=1   → echo $rot
 *          UPDATE motor SET slot='0' WHERE no=1  (always reset, every call)
 *
 *        Response body:
 *          "1" → slot 1 allocated (eligible to open gate)
 *          "2" → slot 2 allocated (eligible to open gate)
 *          "0" → nothing pending  (no action)
 *
 *  [2] Android App → GET /parking/android_db_pool/status.php?n1=<username>
 *        PHP does:
 *          SELECT slot_no FROM booking_up WHERE name='<username>'
 *          UPDATE motor SET slot=slot_no WHERE no=1
 *        This queues the command; ESP32 picks it up on next POST cycle.
 *
 *  Hardware in THIS build (vs original node.ino):
 *    Original used ESP32 receiving sensor JSON over UART from a secondary MCU.
 *    Here: ESP32 reads IR sensors directly → same POST format, no UART needed.
 * =====================================================================
 */

#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <HTTPClient.h>

// =====================================================================
//  CONFIG — edit before flashing
// =====================================================================
const char* WIFI_SSID  = "YOUR_WIFI_SSID";
const char* WIFI_PASS  = "YOUR_WIFI_PASSWORD";
const char* SERVER_IP  = "192.168.x.x";   // ← XAMPP machine IP
const int   SERVER_PORT = 80;

// =====================================================================
//  PIN DEFINITIONS
// =====================================================================
const int IR_PIN_A  = 17;  // Entry gate sensor (sensor1 in PHP)
const int IR_PIN_B  = 16;  // Parking slot 1    (sensor2 in PHP)
const int IR_PIN_C  = 18;  // Parking slot 2    (sensor3 in PHP)
const int SERVO_PIN = 13;  // Gate servo

// =====================================================================
//  SERVO CONFIG
// =====================================================================
Servo gateServo;
const int GATE_CLOSED = 180;
const int GATE_OPEN   = 90;

// Speed control: ms delay between each 1-degree step.
//   2ms  = fast   (~180ms full sweep)
//   5ms  = smooth (~450ms full sweep)  ← recommended
//   15ms = slow   (~1.35s full sweep)
const int SERVO_OPEN_SPEED_MS  = 5;   // ← tune this
const int SERVO_CLOSE_SPEED_MS = 8;   // ← tune this

// =====================================================================
//  GATE / TIMER STATE
// =====================================================================
bool          isGateOpen       = false;
bool          clearTimerActive = false;
unsigned long clearTimerStart  = 0;
const unsigned long CLOSE_DELAY_MS = 3000; // how long to wait after path clears
String pendingSlotCommand = "0";

// =====================================================================
//  POST TIMING
//  Original node.ino used delay(10000) — 10s between uploads.
//  We use 2s for more responsive slot command pickup.
// =====================================================================
unsigned long lastPostTime = 0;
const unsigned long POST_INTERVAL_MS = 2000;


// =====================================================================
//  HELPERS
// =====================================================================
String serverBase() {
  return String("http://") + SERVER_IP + ":" + SERVER_PORT + "/parking";
}

void connectWiFi() {
  Serial.print("[WiFi] Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  unsigned long t = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - t > 15000) {
      Serial.println("\n[WiFi] Timeout — restarting");
      ESP.restart();
    }
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("[WiFi] Connected → ");
  Serial.println(WiFi.localIP());
}


// =====================================================================
//  SERVO — speed-controlled open  (GATE_CLOSED → GATE_OPEN)
// =====================================================================
void openGate() {
  if (isGateOpen) return;
  Serial.println("[GATE] Opening...");
  for (int pos = GATE_CLOSED; pos >= GATE_OPEN; pos--) {
    gateServo.write(pos);
    delay(SERVO_OPEN_SPEED_MS);
  }
  isGateOpen = true;
  Serial.println("[GATE] OPEN");
}

// =====================================================================
//  SERVO — speed-controlled close  (GATE_OPEN → GATE_CLOSED)
// =====================================================================
void closeGate() {
  Serial.println("[GATE] Closing...");
  for (int pos = GATE_OPEN; pos <= GATE_CLOSED; pos++) {
    gateServo.write(pos);
    delay(SERVO_CLOSE_SPEED_MS);
  }
  isGateOpen       = false;
  clearTimerActive = false;
  Serial.println("[GATE] CLOSED");
}


// =====================================================================
//  API — POST sensor states, receive motor.slot command
//
//  Verified against Sensor_data.php:
//    - Payload must be form-urlencoded (not JSON)
//    - All three params must be non-empty or PHP skips the logic
//    - Response is always the echoed $rot value (motor.slot before reset)
//    - motor.slot is reset to '0' EVERY call, so "0" = nothing pending
// =====================================================================
String postSensorData(bool gateCar, bool s1, bool s2) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[HTTP] WiFi lost, skip POST");
    return "0";
  }

  HTTPClient http;
  String url = serverBase() + "/android_db_pool/Sensor_data.php";
  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.setTimeout(5000);

  // Must send all three — PHP checks !empty() on all three before acting
  String body;
  body  = "sensor1=" + String(gateCar ? "p1s1on" : "p1s1off");
  body += "&sensor2=" + String(s1     ? "p1s2on" : "p1s2off");
  body += "&sensor3=" + String(s2     ? "p1s3on" : "p1s3off");

  Serial.println("[HTTP] POST → " + url);
  Serial.println("[HTTP] " + body);

  int    code = http.POST(body);
  String resp = "0";

  if (code == HTTP_CODE_OK) {
    resp = http.getString();
    resp.trim();
    Serial.println("[HTTP] motor.slot response: \"" + resp + "\"");
  } else {
    Serial.printf("[HTTP] POST failed, code=%d\n", code);
  }

  http.end();
  return resp;
}


// =====================================================================
//  Handle motor.slot command from server
//
//  Verified from Sensor_data.php + status.php:
//    status.php writes booking_up.slot_no → motor.slot
//    slot_no values are "1", "2" in this two-slot build
//    "0" means nothing was queued (default / already consumed)
// =====================================================================
void handleSlotCommand(const String& cmd) {
  if (cmd == "0" || cmd == "") return; // nothing pending

  if (cmd == "1" || cmd == "2") {
    pendingSlotCommand = cmd;
    Serial.println("[CMD] Slot " + cmd + " booked → waiting for gate sensor");
  } else {
    Serial.println("[CMD] Unknown response: \"" + cmd + "\" — ignoring");
  }
}


// =====================================================================
//  SETUP
// =====================================================================
void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(IR_PIN_A, INPUT_PULLUP);
  pinMode(IR_PIN_B, INPUT_PULLUP);
  pinMode(IR_PIN_C, INPUT_PULLUP);

  // Anti-glitch: write position BEFORE attach
  gateServo.write(GATE_CLOSED);
  gateServo.attach(SERVO_PIN, 500, 2400);
  gateServo.write(GATE_CLOSED);
  delay(600); // let servo physically reach closed position

  connectWiFi();

  Serial.println("==============================");
  Serial.println(" Smart Parking System Ready");
  Serial.printf("  Open  speed : %d ms/deg\n", SERVO_OPEN_SPEED_MS);
  Serial.printf("  Close speed : %d ms/deg\n", SERVO_CLOSE_SPEED_MS);
  Serial.printf("  POST interval: %lums\n",    POST_INTERVAL_MS);
  Serial.println(" Gate: CLOSED");
  Serial.println("==============================");
}


// =====================================================================
//  LOOP
// =====================================================================
void loop() {
  // --- Read sensors ---
  // INPUT_PULLUP: HIGH = beam broken = car/object present
  // If your IR module outputs HIGH when path is CLEAR, flip to == LOW
  bool carAtGate = (digitalRead(IR_PIN_A) == HIGH);
  bool slot1Busy = (digitalRead(IR_PIN_B) == HIGH);
  bool slot2Busy = (digitalRead(IR_PIN_C) == HIGH);

  // --- Periodic server POST (sensor upload + motor.slot poll) ---
  unsigned long now = millis();
  if (now - lastPostTime >= POST_INTERVAL_MS) {
    lastPostTime = now;
    String cmd = postSensorData(carAtGate, slot1Busy, slot2Busy);
    handleSlotCommand(cmd);
  }

  // --- Final flow: gate opens only if booking command exists AND gate sensor sees a car ---
  if (!isGateOpen && pendingSlotCommand != "0" && carAtGate) {
    Serial.println("[FLOW] Car at gate + valid slot command " + pendingSlotCommand + " → opening");
    openGate();
    pendingSlotCommand = "0";
  }

  if (isGateOpen) {
    if (carAtGate) {
      clearTimerActive = false;
    } else {
      if (!clearTimerActive) {
        clearTimerStart  = millis();
        clearTimerActive = true;
        Serial.println("[GATE] Path clear — closing in " + String(CLOSE_DELAY_MS / 1000) + "s");
      } else if (millis() - clearTimerStart >= CLOSE_DELAY_MS) {
        closeGate();
      }
    }
  }

  delay(50);
}
