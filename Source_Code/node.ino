#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

SoftwareSerial mySerial(D8,D7);
SoftwareSerial nodemcu(D3,D2);

const char* serverName = "http://10.100.70.36/parking/android_db_pool/Sensor_data.php";  

String apiKeyValue = "tPmAT5Ab3j7F9";

 
WiFiClient client;
WiFiServer server(80); 



void setup()
{
  // Debug console
  Serial.begin(115200);
  nodemcu.begin(9600);
  mySerial.begin(9600);
  WiFi.begin("Nord 3","daksh0708");
  while (!Serial)continue;

  while(WiFi.status() != WL_CONNECTED)
  {
  Serial.println("..");
  delay(200);  
  }
  Serial.println();
  Serial.println("NodeMCU is connected");
  Serial.println(WiFi.localIP());

  //client = server.available();
 
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
    
    http.begin(client,"http://10.100.70.36/parking/android_db_pool/Sensor_data.php");
    
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    String httpRequestData = "sensor1=" + sensor1 + "&sensor2=" + sensor2 + "&sensor3=" + sensor3 ;
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
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
