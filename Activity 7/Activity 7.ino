#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include <ArduinoJson.h>

const int pins[] = { 14, 12, 13 };
const int length = sizeof(pins) / sizeof(int);  //get the length of the array
const int btnPins[] = {15, 4, 5};
const int btnLength = sizeof(btnPins) / sizeof(int);


const unsigned long event_interval = 100;
unsigned long prev_time = 0;


const char* WIFINAME = "Redmi Note 7";
const char* PASS = "joshuazion";

const char* serverName = "http://192.168.254.101/data.json";

String response;
int responseArr[3];

WiFiServer server(80);



int btn_val1 = 0;
int btn_state1 = 0;

int btn_val2 = 0;
int btn_state2 = 0;

int btn_val3 = 0;
int btn_state3 = 0;

void post_req(const char* serverName, int value1, int value2, int value3) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverName);
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<48> doc;
    doc["led1"] = value1;
    doc["led2"] = value2;
    doc["led3"] = value3;
    String json;
    // Serial.println(json);
    serializeJson(doc, json);
    Serial.println(json);
    //int httpCode = http.POST(json);
    //if (httpCode > 0) {
    //  String response = http.getString();
    //  Serial.println("Response: " + response);
    //} else {
    //  Serial.println("Error: " + http.errorToString(httpCode));
    //}
    //http.end();
  }
}



String get_req(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);
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

void setup() {
  Serial.begin(115200);


  WiFi.begin(WIFINAME, PASS);


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

  for (int pin = 0; pin < length; pin++) {  //set all pins pinMODE to OUTPUT
    pinMode(pins[pin], OUTPUT);
  }

  for (int pin = 0; pin < btnLength; pin++) {  //set all btnPins pinMODE to INPUT
    pinMode(btnPins[pin], INPUT);
  }
}


void loop() {
  unsigned long curr_time = millis();


  if (curr_time - prev_time >= event_interval) {

    btn_state1 = digitalRead(btnPins[0]);
    btn_state2 = digitalRead(btnPins[1]);
    btn_state3 = digitalRead(btnPins[2]);

    if (btn_state1 == 1) {
      btn_state1 = 0;
      digitalWrite(pins[0], btn_val1);
      btn_val1 = !btn_val1;
      post_req(serverName, btn_val1, btn_val2, btn_val3);
      Serial.println("btn1");
    }

    else if (btn_state2 == 1) {
      btn_state2 = 0;
      digitalWrite(pins[1], btn_val2);
      btn_val2 = !btn_val2;
      post_req(serverName, btn_val1, btn_val2, btn_val3);
      Serial.println("btn2");
    }

    else if (btn_state3 == 1) {
      btn_state3 = 0;
      digitalWrite(pins[2], btn_val3);
      btn_val3 = !btn_val3;
      post_req(serverName, btn_val1, btn_val2, btn_val3);
      Serial.println("btn3");
    }

    // response = get_req(serverName);
    // JSONVar myObject = JSON.parse(response);
    // // JSON.typeof(jsonVar) can be used to get the type of the var
    // if (JSON.typeof(myObject) == "undefined") {
    //   Serial.println("Parsing input failed!");
    //   return;
    // }


    // Serial.print("JSON object = ");
    // Serial.println(myObject);

    // // myObject.keys() can be used to get an array of all the keys in the object
    // JSONVar keys = myObject.keys();

    // for (int i = 0; i < 3; i++) {
    //   JSONVar value = myObject[keys[i]];
    //   Serial.print(keys[i]);
    //   Serial.print(" = ");
    //   Serial.println(value);
    //   responseArr[i] = int(value);
    // }
    // Serial.println(responseArr[2]);

    // digitalWrite(pins[0], responseArr[0]);
    // digitalWrite(pins[1], responseArr[1]);
    // digitalWrite(pins[2], responseArr[2]);

    prev_time = curr_time;
  }
}