#include "Arduino.h"
#include "./dht.h"

void displayTemp(float tempc, float humid){
    Serial.print("Temp in C = ");
    Serial.println(tempc);
    Serial.print("Humidity = ");
    Serial.println(humid);
}
