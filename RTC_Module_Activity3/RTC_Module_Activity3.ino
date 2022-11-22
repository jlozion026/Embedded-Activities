#include <LiquidCrystal_I2C.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <Keypad.h>
// int year, month, day, hour, minute, second;
int hour, minute, second;
//0x3F or 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);   //LCD Object

ThreeWire myWire(13, 10, 12);        // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);    // RTC Object

const unsigned long event_interval = 1000;
unsigned long prev_time = 0;


const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};


byte row_pins[ROWS] = {9, 8, 7, 6};
byte col_pins[COLS] = {5, 4, 3, 2};

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), row_pins, col_pins, ROWS, COLS);
const byte bzz_pin = 11;

void buzz(){
    for(byte i = 0; i <= 10; i++){
        for(int buzz = 0; buzz < 2; buzz++){
            tone(bzz_pin, 1000);
            delay(200);
            noTone(bzz_pin);
            delay(200);
        }
        delay(200);
    }
}

bool alarm(){
  RtcDateTime now = Rtc.GetDateTime();
  if(now.Hour() == hour &&
     now.Minute() == minute &&
     now.Second() == second
  ){
    return true;
  }
  
  //String dateFormat = "Curr Date " + String(now.Year()) + "/" + String(now.Month()) + "/" + String(now.Day());
  String timeFormat = "Curr Time " + String(now.Hour()) + "/" + String(now.Minute()) + "/" + String(now.Second());

  //String inputDate = "Input Date " + String(year) + "/" + String(month) + "/" + String(day);
  String inputTime = "Input Time " + String(hour) + "/" + String(minute) + "/" + String(second);


  //Serial.println(dateFormat);
  Serial.println(timeFormat);

  //Serial.println(inputDate);
  Serial.println(inputTime);
  return false;
  
}


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
  // RtcDateTime currentTime = RtcDateTime(__DATE__ , __TIME__);
  // Rtc.SetDateTime(currentTime);
  pinMode(bzz_pin, OUTPUT);

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
    lcd.print(now.Second());
    prev_time = curr_time;

    char c = customKeypad.getKey();
    if (c == '*') {
      // lcd.clear();
      // lcd.setCursor(0, 0);
      // lcd.print("Enter Year: ");
      // year = getInput();

      // lcd.clear();
      // lcd.setCursor(0, 0);
      // lcd.print("Enter Month: ");
      // month = getInput();

      // lcd.clear();
      // lcd.setCursor(0, 0);
      // lcd.print("Enter DayOfWeek: ");
      // day = getInput();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Hours: ");
      hour = getInput();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Minutes: ");
      minute = getInput();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Seconds: ");
      second = getInput();

      // RtcDateTime newTime = RtcDateTime(year, month, day, hour, minute, second);
      // Rtc.SetDateTime(newTime);

    }
      if(alarm()){
        Serial.println("Alarm");
        buzz();
      } else {
        Serial.println("No Alarm");
    }
  } 
}
