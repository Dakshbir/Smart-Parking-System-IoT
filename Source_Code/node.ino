#include <HardwareSerial.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

HardwareSerial mySerial(1);
HardwareSerial nodemcu(2);

const char* serverName = "http://10.100.70.36/parking/android_db_pool/Sensor_data.php";  

WiFiClient client;

void setup()
{
  // Debug console
  Serial.begin(115200);
  // ESP32 UART mapping:
  // nodemcu channel -> RX GPIO16, TX GPIO17
  // mySerial channel -> RX GPIO26, TX GPIO25
  nodemcu.begin(9600, SERIAL_8N1, 16, 17);
  mySerial.begin(9600, SERIAL_8N1, 26, 25);
  WiFi.begin("Nord 3","daksh0708");

  while(WiFi.status() != WL_CONNECTED)
  {
  Serial.println("..");
  delay(200);  
  }
  Serial.println();
  Serial.println("NodeMCU is connected");
  Serial.println(WiFi.localIP());

}
 
void loop()
{
  //Serial.println(host);
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(nodemcu);
  
   
  if (data == JsonObject::invalid())
  {
  jsonBuffer.clear();
  return;  
  } 

  Serial.println("Jeson Object Recived");
  String sensor1 = data["p1"];
  Serial.println(sensor1);
  String sensor2 = data["p2"];
  Serial.println(sensor2);
  String sensor3 = data["p3"];
  Serial.println(sensor3);
if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
  http.begin(client, serverName);
    
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    String httpRequestData = "sensor1=" + sensor1 + "&sensor2=" + sensor2 + "&sensor3=" + sensor3 ;
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    int httpResponseCode = http.POST(httpRequestData);
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      if (httpResponseCode == 200) { Serial.println("Values uploaded successfully."); Serial.println(httpResponseCode); 
String webpage = http.getString();    // Get html webpage output and store it in a string
Serial.println(webpage + "\n"); 

/*StaticJsonBuffer<1000> jsonBuffer;
JsonObject& ard_data = jsonBuffer.createObject();
ard_data["d1"] = 100;
ard_data.printTo(mySerial);
jsonBuffer.clear();*/
JsonObject& ard_data = jsonBuffer.createObject();
  ard_data["d1"] = webpage;
  ard_data.printTo(mySerial);
  jsonBuffer.clear();
}
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 15 seconds
//StaticJsonBuffer<1000> jsonBuffer;
  delay(10000);  
} 
  
 
/*String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
 
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}*/
