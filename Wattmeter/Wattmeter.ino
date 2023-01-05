#include "ACS712.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
/*
  This example shows how to measure the power consumption
  of devices in 230V electrical system
  or any other system with alternative current
*/

// We have 30 amps version sensor connected to A0 pin of arduino
// Replace with your version if necessary
ACS712 sensor(ACS712_20A, A0);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();  
  lcd.backlight();
  lcd.print("Current Sensor");
  // calibrate() method calibrates zero point of sensor,
  // It is not necessary, but may positively affect the accuracy
  // Ensure that no current flows through the sensor at this moment
  // If you are not sure that the current through the sensor will not leak during calibration - comment out this method
  Serial.println("Calibrating... Ensure that no current flows through the sensor at this moment");
  sensor.calibrate();
  Serial.println("Done!");

  delay(2000);
}

void loop() {
  // We use 230V because it is the common standard in European countries
  // Change to your local, if necessary
  float U = 230;

  // To measure current we need to know the frequency of current
  // By default 50Hz is used, but you can specify desired frequency
  // as first argument to getCurrentAC() method, if necessary
  float I = sensor.getCurrentAC();

  // To calculate the power we need voltage multiplied by current
  float P = U * I;

  Serial.println(String("I = ") + I + " A");
  Serial.println(String("P = ") + P + " Watts");
  
  lcd.clear();
  lcd.setCursor (0,0); 
  lcd.print("Current: ");
  lcd.print(I,3);
  lcd.print(" A");
  
  lcd.setCursor (0,1); 
  lcd.print("Power: ");
  lcd.print(P,3);
  lcd.print(" W");





  delay(1000);
}