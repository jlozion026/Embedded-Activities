#include "ACS712.h"
#include <Wire.h> 
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
/*
  This example shows how to measure the power consumption
  of devices in 230V electrical system
  or any other system with alternative current
*/

// We have 30 amps version sensor connected to A0 pin of arduino
// Replace with your version if necessary
ACS712 sensor(ACS712_05B, A0);
const int bulbSwitch = 5;
const int fanSwitch = 15;
float totalConsumption = 21;
float totalCostConsumption = 10;
float V = 220;
int address0 = 0;  // starting address in EEPROM to store power
int address1 = 1;

const unsigned long event_interval = 1000;
unsigned long prev_time = 0;

const char* WIFINAME = "Redmi Note 7";
const char* PASS = "joshuazion";

const char* bulbEndpoint = "http://192.168.43.81/data.json";
const char* fanEndpoint = "http://192.168.43.81/fan.json";
const char* postEndpoint = "http://192.168.43.81/action.php";

String response;
int responseArr[2];

String get_req(const char* bulbEndpoint) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, bulbEndpoint);
  int httpCode = http.GET();

  String payload = "{}";


  if (httpCode > 0) {
    payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpCode);
  }

  http.end();
  return payload;
}

String get_fan_req(const char* fanEndpoint) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, fanEndpoint);
  int httpCode = http.GET();

  String payload = "{}";


  if (httpCode > 0) {
    payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpCode);
  }

  http.end();
  return payload;
}

int switch_bulb () {
  int payload = 0;
  response = get_req(bulbEndpoint);
    JSONVar myObject = JSON.parse(response);
    // JSON.typeof(jsonVar) can be used to get the type of the var
    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
      return 0;
    }

    JSONVar keys = myObject.keys();

    // for (int i = 0; i < 2; i++) {
      JSONVar value = myObject[keys[0]];
      Serial.println(value);
      // Serial.print(keys[i]);
      // Serial.print(" = ");
      // Serial.println(value);
      return (value) ? 1 : 0;
      // responseArr[i] = (value) ? 1 : 0 ;
    // }
}

int switch_fan () {
  response = get_fan_req(fanEndpoint);
    JSONVar myObject = JSON.parse(response);
    // JSON.typeof(jsonVar) can be used to get the type of the var
    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
      return 0;
    }

    JSONVar keys = myObject.keys();

    // for (int i = 0; i < 2; i++) {
      JSONVar value = myObject[keys[0]];
      Serial.println(value);
      // Serial.print(keys[i]);
      // Serial.print(" = ");
      // Serial.println(value);
      return (value) ? 1 : 0;
      // responseArr[i] = (value) ? 1 : 0 ;
    // }
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);  // initialize EEPROM with size of 512 bytes
   WiFi.begin(WIFINAME, PASS);
   pinMode(bulbSwitch, OUTPUT);
   pinMode(fanSwitch, OUTPUT);
   
  Serial.print("Connecting To ");
  Serial.println(WIFINAME);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // If you are not sure that the current through the sensor will not leak during calibration - comment out this method
  sensor.calibrate();
  Serial.println("Done!");
 // sensor.setSensitivity(185);
  delay(2000);
}

void loop() {

  unsigned long curr_time = millis();
  

  if (curr_time - prev_time >= event_interval) {
    float I = sensor.getCurrentAC(50);
    Serial.print("Current Reading: ");
    Serial.print(I,9);
    Serial.println(" ampere");

    float P_watt = (V * I);
    float P_kw = P_watt/1000.000000;
    Serial.print("Total Power: ");
    Serial.print(P_kw,9);
    Serial.println(" kilowatt");
    
    float per_sec = 0.0002777777;
    totalConsumption += (P_kw * per_sec);

    Serial.print("Total energy consumption: ");
    Serial.print(totalConsumption, 9);
    Serial.println(" kilowatt hours");
    EEPROM.write(address0, totalConsumption);
    int energy = EEPROM.read(address0);
    Serial.println(energy);

    totalCostConsumption += ((P_kw * per_sec) * 5.399);

    Serial.print("Total cost consumption: ");
    Serial.print(totalCostConsumption, 3);
    Serial.println("Php");
    EEPROM.write(address1, totalCostConsumption);
    int cost = EEPROM.read(address1);
    Serial.println(cost);

    digitalWrite(bulbSwitch, switch_bulb());
    digitalWrite(fanSwitch, switch_fan());
    prev_time = curr_time;
  }
  if (curr_time - prev_time >= 60000) {
    EEPROM.write(address0, 0);
    Serial.println(EEPROM[0]);

    EEPROM.write(address1, 0);
    Serial.println(EEPROM[1]);


    prev_time = curr_time;
  }
 
}