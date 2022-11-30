#include <Servo.h>
#include <Wire.h>

Servo servo;

#define ADDR 2


void format_data(int sm, int angle){
    char data[50];
    sprintf(data, "angle: %d,\n press: %d.", angle, sm);
    Serial.println(data);
}

void rotate(int Press){
  int angle = Wire.read();
  servo.write(angle);

  format_data(Press, angle);
}

void setup(){
  Wire.begin(ADDR);
  servo.attach(5);
  Wire.onReceive(rotate);

  Serial.begin(9600);
}

void loop(){
  delay(1000);
}

