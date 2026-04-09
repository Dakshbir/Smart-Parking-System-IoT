#include <SoftwareSerial.h>
#include <ArduinoJson.h> 
#include <Servo.h>

#define SERVO_PIN 9
#define SERVO_CLOSED_ANGLE 0
#define SERVO_OPEN_ANGLE 90
// Set to true if you currently have only one IR sensor connected (pin D4).
// In this mode, one sensor reading is mirrored to all 3 slot states for testing.
#define SINGLE_SENSOR_MODE false

SoftwareSerial nodemcu(2,3);
SoftwareSerial mySerial(7,8);
Servo gateServo;

int parking1_slot1_ir_s = 4;
int parking1_slot2_ir_s = 5;
int parking1_slot3_ir_s = 6;
 
/*int parking2_slot1_ir_s = 7;
int parking2_slot2_ir_s = 8;
int parking2_slot3_ir_s = 9;*/
 
String sensor1; 
String sensor2; 
String sensor3; 
/*String sensor4; 
String sensor5; 
String sensor6; */
 
 String slot;

void setup()
{
Serial.begin(115200); 
nodemcu.begin(9600); 
mySerial.begin(9600);
pinMode(parking1_slot1_ir_s, INPUT);
pinMode(parking1_slot2_ir_s, INPUT);
pinMode(parking1_slot3_ir_s, INPUT);
/*
 
pinMode(parking2_slot1_ir_s, INPUT);
pinMode(parking2_slot2_ir_s, INPUT);
pinMode(parking2_slot3_ir_s, INPUT);*/
gateServo.attach(SERVO_PIN);
gateServo.write(SERVO_CLOSED_ANGLE);

}

void loop() {
StaticJsonBuffer<1000> jsonBuffer;
JsonObject& data = jsonBuffer.createObject();

p1slot1(); 
if (SINGLE_SENSOR_MODE) {
  // Mirror slot1 sensor into slot2 and slot3 labels for quick end-to-end testing.
  sensor2 = (sensor1 == "p1s1on") ? "p1s2on" : "p1s2off";
  sensor3 = (sensor1 == "p1s1on") ? "p1s3on" : "p1s3off";
} else {
  p1slot2();
  p1slot3();
}

data["p1"] = sensor1;
data["p2"] = sensor2;
data["p3"] = sensor3;
/*p1slot3(); 
 
p2slot1();
p2slot2();
p2slot3();*/
 
  
  
   data.printTo(nodemcu);

  if(nodemcu.available()>0){
    Serial.println("Connected");
  }

//StaticJsonBuffer<1000> jsonBuffer;
JsonObject& ard_data = jsonBuffer.parseObject(mySerial);
//   
//
Serial.println("Jeson Object Recived");
String a = ard_data["d1"];
Serial.println(a);

slot = a;

   Serial.println(sensor1); 
   //nodemcu.println(data);
   delay(2000); // 2 seconds
   jsonBuffer.clear();
//digitalWrite(parking1_slot1_ir_s, HIGH); 
//digitalWrite(parking1_slot2_ir_s, HIGH); 
/*digitalWrite(parking1_slot3_ir_s, HIGH);
 
digitalWrite(parking2_slot1_ir_s, HIGH);
digitalWrite(parking2_slot2_ir_s, HIGH);
digitalWrite(parking2_slot3_ir_s, HIGH);*/

// Open gate for a valid booked slot and auto-close after delay.
if(slot == "1" || slot == "2" || slot == "3"){
  operateGate();
  slot = "0";
}
  
}
 
//P1slot1 is a user defined function, it has no return type and it doesn not take any argument as the input.  if there is a car infront of the sensor it gives digital logic 0, and if no car then it give digital logic 1, depending on this, then we store p1s1on or p1s1off.  The same mechanism is used for all the other infrared sensors.
 
void p1slot1() // parkng 1 slot1
{
  if( digitalRead(parking1_slot1_ir_s) == LOW) 
  {
  sensor1 = "p1s1on"; // parking1 slot1 
 delay(200); 
  } 
if( digitalRead(parking1_slot1_ir_s) == HIGH)
{
  sensor1 = "p1s1off";  
 delay(200);  
}
 
}
 
 void p1slot2() // parking 1 slot2
{
  if( digitalRead(parking1_slot2_ir_s) == LOW) 
  {
  sensor2 = "p1s2on"; 
  delay(200); 
  }
if( digitalRead(parking1_slot2_ir_s) == HIGH)  
  {
  sensor2 = "p1s2off";  
 delay(200);
  } 
}

void p1slot3() // parking 1 slot3
{
  if( digitalRead(parking1_slot3_ir_s) == LOW) 
  {
  sensor3 = "p1s3on"; 
  delay(200); 
  }
if( digitalRead(parking1_slot3_ir_s) == HIGH)  
  {
  sensor3 = "p1s3off";  
 delay(200);
  } 
}

 void operateGate(){
  gateServo.write(SERVO_OPEN_ANGLE);
  delay(5000);
  gateServo.write(SERVO_CLOSED_ANGLE);
  delay(1000);
}
 
/*void p1slot3() // parking 1 slot3
{
  if( digitalRead(parking1_slot3_ir_s) == LOW) 
  {
  sensor3 = "p1s3on"; 
  delay(200); 
  }
if( digitalRead(parking1_slot3_ir_s) == HIGH)  
  {
  sensor3 = "p1s3off";  
 delay(200);
  } 
}
 
 
// now for parking 2
 
void p2slot1() // parking 1 slot3
{
  if( digitalRead(parking2_slot1_ir_s) == LOW) 
  {
  sensor4 = "p2s1on"; 
  delay(200); 
  }
if( digitalRead(parking2_slot1_ir_s) == HIGH)  
  {
  sensor4 = "p2s1off";  
 delay(200);
  } 
}
 
 
void p2slot2() // parking 1 slot3
{
  if( digitalRead(parking2_slot2_ir_s) == LOW) 
  {
  sensor5 = "p2s2on"; 
  delay(200); 
  }
if( digitalRead(parking2_slot2_ir_s) == HIGH)  
  {
  sensor5 = "p2s2off";  
 delay(200);
  } 
}
 
 
void p2slot3() // parking 1 slot3
{
  if( digitalRead(parking2_slot3_ir_s) == LOW) 
  {
  sensor6 = "p2s3on"; 
  delay(200); 
  }
if( digitalRead(parking2_slot3_ir_s) == HIGH)  
  {
  sensor6 = "p2s3off";  
 delay(200);
  } 
}
}*/
