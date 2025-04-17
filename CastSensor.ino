
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

int sensor = 0;
double offset=0;
int levelthresholds[]={-1,7,14,21,30,40,50,65,80,95,115};
String feedback[]={"Doing well!","Nice and easy","Steady now...","Careful there!","Whoa, lighten up","Too much weight!","Uh-oh...","You're pushing it","Danger zone!","Stop! Rest foot!","STOP"};



LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup(){ 
  lcd.init();
  lcd.backlight();
  lcd.clear();
  pinMode(A0, INPUT);
  offset=Calibrate(lcd,200);
  Serial.begin(9600);
}

void loop(){ 
  
  //We need to convert the sensor readings into pounds
  lcdPrintSensor(offset);
  delay(500);
}
void lcdPrintSensor(double offset){
  sensor = analogRead(A0)-offset;
  Serial.println(sensor);
  int level;
  char* message[16];
  boolean push=false;
  for(int i=sizeof(levelthresholds) / sizeof(levelthresholds[0])-1;i>=0;i--){
    if(sensor>levelthresholds[i]){
      lcd.clear();
      String firstLine="Level "+String(i+1)+": "+String(sensor);
      lcd.setCursor(0,0);
      lcd.print(firstLine);
      lcd.setCursor(0,1);
      lcd.print(String(feedback[i]));
      break;
    }
  }
}
double Calibrate(LiquidCrystal_I2C lcd, int readings){
  //Actually does stuff calibration
  int total=0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibrating.");
  for(int i=0;i<readings/3;i++){
    total+=analogRead(A0);
    delay(15);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibrating..");
  for(int i=0;i<readings/3;i++){
    total+=analogRead(A0);
    delay(15);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibrating...");
  for(int i=0;i<readings/3;i++){
    total+=analogRead(A0);
    delay(15);
  }
  return total/readings;
}
