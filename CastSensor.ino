#include "arduino_secrets.h"


#include <LiquidCrystal_I2C.h>
#include <Wire.h>

int sensor = 0;
double offset=0;

int level1=7; 
int level2=14;
int level3=21;
int level4=30;
int level5=40;
int level6=50;
int level7=65;
int level8=80;
int level9=95;
int level10=115;

char level1Feedback[]="Doing well!";
char level2Feedback[]="Nice and easy";
char level3Feedback[]="Steady now...";
char level4Feedback[]="Careful there!";
char level5Feedback[]="Whoa, lighten up";
char level6Feedback[]="Too much weight!";
char level7Feedback[]="Uh-oh...";
char level8Feedback[]="You're pushing it";
char level9Feedback[]="Danger zone!";
char level10Feedback[]="Stop! Rest foot!";
char level11Feedback[]="STOP";



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
  int level;
  char *message;
  boolean push=false;
  if(sensor>level10){
    level=11;
    push=true;
    message=level11Feedback;
  }
  else if(sensor>level9){
    level=10;
    push=true;
    message=level10Feedback;
  }
  else if(sensor>level8){
    level=9;
    message=level9Feedback;
  }
  else if(sensor>level7){
    level=8;
    message=level8Feedback;
  }
  else if(sensor>level6){
    level=7;
    message=level7Feedback;
  }
  else if(sensor>level5){
    level=6;
    message=level6Feedback;
  }
  else if(sensor>level4){
    level=5;
    message=level5Feedback;
  }
  else if(sensor>level3){
    level=4;
    message=level4Feedback;
  }
  else if(sensor>level2){
    level=3;
    message=level3Feedback;
  }
  else if(sensor>level1){
    level=2;
    message=level2Feedback;
  }
  else{
    level=1;
    message=level1Feedback;
  }
  Serial.println(sensor);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Level");
  lcd.setCursor(6, 0);
  lcd.print(level);
  if(!push){
    lcd.setCursor(7, 0);
    lcd.print(":");
    lcd.setCursor(9, 0);
    lcd.print(sensor);
  }
  else{
    lcd.setCursor(8, 0);
    lcd.print(":");
    lcd.setCursor(10, 0);
    lcd.print(sensor);
  }
  
  lcd.setCursor(0, 1);
  Serial.println(message);
  lcd.print(message);
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
  int numFakeCalibrations=random(0,3);
  for(int i=0; i<=numFakeCalibrations;i++){
    lcd.clear();
    lcd.setCursor(0,0);
    if(i%3==0){
      lcd.print("Calibrating.");
    }
    else if(i%3==1){
      lcd.print("Calibrating..");
    }
    else if(i%3==2){
      lcd.print("Calibrating...");
    }
    delay(random(0,2)*1000);
  }
  return total/readings;
}