#include <LiquidCrystal_I2C.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <Keypad.h>


//0x3F or 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);   //LCD Object

ThreeWire myWire(7, 6, 8);        // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);    // RTC Object

const unsigned long event_interval = 1000;
unsigned long prev_time = 0;


const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};


byte row_pins[ROWS] = {6, 7, 8, 9}; //connect to the row pinouts of the keypad
byte col_pins[COLS] = {3, 4, 5}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), row_pins, col_pins, ROWS, COLS);


int getInput(){
  String data = "";
  lcd.setCursor(0, 1);
  while(1){
    char input = customKeypad.getKey();
    if(input == '#') break;

    if(isDigit(input)){
      data += input;
      lcd.print(input);
    }
  }
  return data.toInt();
}

void reset_time(){
  RtcDateTime currentTime = RtcDateTime(__DATE__ , __TIME__);
  Rtc.SetDateTime(currentTime);
}

void setup ()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();

  Rtc.Begin();

}

void loop () {

  unsigned long curr_time = millis();
  if(curr_time - prev_time >= event_interval){

    RtcDateTime now = Rtc.GetDateTime();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Date: ");
    lcd.print(now.Day());
    lcd.print("/");
    lcd.print(now.Month());
    lcd.print("/");
    lcd.print(now.Year());

    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    lcd.print(now.Hour());
    lcd.print(":");
    lcd.print(now.Minute());
    lcd.print(":");
    lcd.println(now.Second());
    prev_time = curr_time;

    char c = customKeypad.getKey();
    if (c == '*') {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Year: ");
      int year = getInput();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Month: ");
      int month = getInput();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter DayOfWeek: ");
      int day = getInput();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Hours: ");
      int hour = getInput();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Minutes: ");
      int minute = getInput();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Seconds: ");
      int second = getInput();

      RtcDateTime newTime = RtcDateTime(year, month, day, hour, minute, second);
      Rtc.SetDateTime(newTime);

    }
  }
}
