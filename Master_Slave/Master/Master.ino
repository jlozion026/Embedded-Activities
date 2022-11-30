#include <Wire.h>

const int pot_pin1 = A0;
const int pot_pin2 = A1;

const unsigned long event_interval = 1000;
unsigned long prev_time = 0;

int prev_reading_1, prev_reading_2;

void transmit(byte adr, long data){
    Wire.beginTransmission(adr);                                                                             
    Wire.write(data);                                                    
    Wire.endTransmission();                                                  
}

void setup(){
  Wire.begin();
}

void loop(){

  unsigned long curr_time = millis();

  if(curr_time - prev_time >= event_interval){

    int curr_reading_1 = analogRead(pot_pin1);
    int curr_reading_2 = analogRead(pot_pin2);

    if(curr_reading_1 != prev_reading_1){
      int data = map(curr_reading_1, 0, 1023, 0, 180);
      transmit(1, data);

    }
    else{
      int data = map(curr_reading_2, 0, 1023, 0, 180);
      transmit(2, data);
    }
    prev_time = curr_time;
  }
}
