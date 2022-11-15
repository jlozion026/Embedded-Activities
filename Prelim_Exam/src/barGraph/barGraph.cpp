#include "Arduino.h"
#include "./barGraph.h"

byte one_two[] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
};

byte three_four[] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000
};

byte five[] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100
};

byte six[] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110
};

byte seven[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte rest[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
BarGraph::BarGraph(LiquidCrystal_I2C* lcd){
    _lcd = lcd;
}

void BarGraph::begin(){
  _lcd->createChar(0, one_two);
  _lcd->createChar(1, three_four);
  _lcd->createChar(2, five);
  _lcd->createChar(3, six);
  _lcd->createChar(4, seven);
  _lcd->createChar(5, rest);
}

void BarGraph::setPrevBar(int position){
  int counter = 0;
  while(counter != position){
    _lcd->setCursor(counter, 0);
    _lcd->write((byte)4);
    
    counter += 1;
  }
}

void BarGraph::printBar(int position, int val){

  setPrevBar(position);

  _lcd->setCursor(position, 0);

  if (val == 0){
    _lcd->write((byte)5);
  }
  else if(val > 0 && val <= 2){
    _lcd->write((byte)0);
  }
  else if(val > 2 && val < 5){
    _lcd->write((byte)1);
  }
  else if(val == 5){
    _lcd->write((byte)2);
  }
  else if(val == 6){
    _lcd->write((byte)3);
  }
  else if(val == 7){
    _lcd->write((byte)4);
  }
}
