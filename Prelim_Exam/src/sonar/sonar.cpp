#include "Arduino.h"
#include "./sonar.h"

void displaySonarReading(unsigned long sonar_reading){
    Serial.print("Current Reading: ");
    Serial.print(sonar_reading); 
    Serial.println(" cm");
}
