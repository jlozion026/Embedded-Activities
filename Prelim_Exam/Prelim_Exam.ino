#include <EEPROM.h>

#include "src/sonar/sonar.h"
#include "src/barGraph/barGraph.h"
#include "src/dht/dht.h"


#define sensor 2            // S, vcc, gnd
#define DHTTYPE DHT11
DHT dht(sensor, DHTTYPE);

NewPing sonar(7, 8, 7);

const unsigned long event_interval = 2000;
unsigned long prev_time = 0;

const unsigned long level_interval = 2000;
unsigned long level_prev_time = 0;


LiquidCrystal_I2C lcd(0x27, 16, 2);
BarGraph barGraph(&lcd);

const byte CUP_THRESHOLD = 7;



void calc_curr_intake(unsigned long curr_reading, byte water_level){
  if(curr_reading > EEPROM.read(0)){
    // EEPROM.write(1, water_level + sonar_reading);
    byte current_intake = curr_reading - EEPROM[0];
    EEPROM[1] = current_intake + water_level;
    
    Serial.print("Water Level Intake: ");
    Serial.println(EEPROM[1]);
  }
}

void calc_total_cup(){
  if(EEPROM[1] >= CUP_THRESHOLD){
    byte water_diff = EEPROM[1] - CUP_THRESHOLD;
    EEPROM[1] = water_diff; 

    byte cup_counter = EEPROM[2];
    EEPROM[2] = cup_counter + 1;
    Serial.print("Number of Cups: ");
    Serial.println(EEPROM[2]);

    EEPROM[0] = 0;
  }
}



void setup() {
    Serial.begin(9600);

    pinMode(sensor, INPUT);

    dht.begin();

    lcd.init();

    barGraph.begin();

    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("  Prelim Exam ");
    lcd.setCursor(0, 1);
    lcd.print("   A.G.G.N.O.  ");
    lcd.clear();
    delay(2000);
}

void loop() {
    unsigned long curr_time = millis();
    

    if(curr_time - prev_time >= event_interval){
        float temp = dht.readTemperature();;
        float humid = dht.readHumidity();
        
        displayTemp(temp, humid);
        
        prev_time = curr_time;
    }

    unsigned long level_curr_time = millis();
    if(level_curr_time - level_prev_time >= level_interval){
      unsigned long sonar_reading = sonar.ping_cm(); //current reading
      
      displaySonarReading(sonar_reading);

      barGraph.printBar(EEPROM[2], sonar_reading);
      lcd.setCursor(0, 1);
      lcd.print("Total cups:");
      lcd.print(EEPROM[2]);

      // checks if the current reading is >= the previous reading
      if(sonar_reading != EEPROM[0]){
        /*
         **EEPROM address assignment: 
              address 0: previous reading
              address 1: water counter
              address 2: cup counter
        */

        byte water_level = EEPROM[1];
      
        // if curr_reading > water_level: increment 
        calc_curr_intake(sonar_reading, water_level);

        // if water counter == CUP_THRESHOLD: increment cup counter
        calc_total_cup();

        EEPROM[0] = sonar_reading; //save the current reading to EEPROM address 0
     }

      level_prev_time = level_curr_time;
    }
}
