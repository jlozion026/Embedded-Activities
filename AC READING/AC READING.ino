const int VIN = A0; // define the Arduino pin A0 as voltage input (V in)

const float VCC   = 5.00;// supply voltage
const int MODEL = 0;   // enter the model (see above list)s
#include <Robojax_AllegroACS_Current_Sensor.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

Robojax_AllegroACS_Current_Sensor robojax(MODEL,VIN);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void format_curr_power(float current, float power){
    char data[100];
    char str_c[10];
    char str_p[10];
    
    dtostrf(current, 5, 3, str_c);
    dtostrf(power, 5, 3, str_p);
    sprintf(data, "C: %s\nP: %s\n", str_c, str_p);
    Serial.println(data);
}

void setup() {


  Serial.begin(9600);
  lcd.init();  
  lcd.backlight();
  lcd.print("Current Sensor");
  delay(2000);     
}

void loop() {  
   
    float avgCurrent = robojax.getCurrentAverage(1000);
    float avgPower= avgCurrent * 12;

    format_curr_power(avgCurrent, avgPower);

    // lcd.clear();
    // lcd.setCursor (0,0); // set to line 1, char 0  
    // lcd.print("Current: ");
    // lcd.print(avgCurrent,3);
    // lcd.print(" A");
    
    // lcd.setCursor (0,1); // set to line 1, char 0  
    // lcd.print("Power: ");
    // lcd.print(avgPower);
    // lcd.print(" W");

  delay(1500);
}
 