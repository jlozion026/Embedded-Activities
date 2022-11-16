#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


class BarGraph{
  public:
    BarGraph(LiquidCrystal_I2C*);
    void begin();
    void printBar(int position, int val);
    void setPrevBar(int val);

  private:
    LiquidCrystal_I2C* _lcd;
};

