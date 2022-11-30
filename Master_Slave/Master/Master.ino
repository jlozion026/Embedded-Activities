#include <Wire.h>

const int pot_pin1 = A0;
const int pot_pin2 = A1;
const int pot_pin3 = A2;

const unsigned long event_interval = 50;
unsigned long prev_time = 0;

int prev_reading_1 = 0; 
int prev_reading_2 = 0;
int prev_reading_3 = 0;

void transmit(byte adr, long data){
    Wire.beginTransmission(adr);                                                                             
    Wire.write(data);                                                    
    Wire.endTransmission();                                                  
}

void format_data(int sm, int angle){
    char data[50];
    sprintf(data, "Curr: %d,\n Prev: %d.", angle, sm);
    Serial.println(data);
}


void setup(){
  Wire.begin();
  Serial.begin(9600);
}

void loop(){

  unsigned long curr_time = millis();

  if(curr_time - prev_time >= event_interval){

    int curr_reading_1 = analogRead(pot_pin1);
    int curr_reading_2 = analogRead(pot_pin2);
    int curr_reading_3 = analogRead(pot_pin3);
    

    if(curr_reading_1 != prev_reading_1){
      int data = map(curr_reading_1, 0, 1023, 0, 180);
      transmit(1, data);
      //format_data(curr_reading_1, prev_reading_1);

      prev_reading_1 = curr_reading_1;
    }

    if(curr_reading_2 != prev_reading_2){
      int data = map(curr_reading_2, 0, 1023, 0, 180);
      transmit(2, data);
      //format_data(curr_reading_2, prev_reading_2);
    
      prev_reading_2 = curr_reading_2;
    }

    if(curr_reading_3 != prev_reading_3){
      int data = map(curr_reading_3, 0, 1023, 0, 180);
      transmit(3, data);
      format_data(curr_reading_3, prev_reading_3);

      prev_reading_3 = curr_reading_3;
    }

    prev_time = curr_time;
  }
}
