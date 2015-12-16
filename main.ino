#include <Wire.h>
#define DELAY_TIME 1
#include <OnewireKeypad.h>
char KEYS[]= {
  '7','8','9','+','a','b',   //'A',
  '4','5','6','c','<','d',   //'B',
  '1','2','3','e','f','g',   //'C'
  'Z','0','C','.','h','j',   //'D',
  '4','o','6','*','k','l',   //'E',
  'z','x','c','v','-','q',   //'F'
                }; 
OnewireKeypad <Print, 36 > KP2(Serial, KEYS, 6, 6, A6, 4700, 1000 );

#include <LiquidCrystal.h>
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
#define step_pin 9 // Pin 3 connected to Steps pin on EasyDriver
#define dir_pin 10   // Pin 2 connected to Direction pin
#define SLEEP 7     // Pin 7 connected to SLEEP pin
#define ZERO_POS 2//pin punct 0 
#define MAX_STEPS  33320//se va inmulti cu 1000
int direction;    // Variable to set Rotation (CW-CCW) of the motor
int currentPos=0,targetPos=0;
// variable to store the length of received bytes
int incomingLen = 0;
// variable to store the actual bytes read from the receive buffer
byte readLen = 0;
// variable to store incoming bytes of ascii chars
char buffer[64];


int strToInt(char AStr[], byte ALen)
{
  int Result = 0;
  int c;
  for (int i=0; i < ALen; i++)
  {
    // calc the ordinal value of each character
    c = int(AStr[i] - '0');
    // check if the current char is number, otherwise return -1
    if (c < 0 || c > 9)
      return -1;
   
    // adjust the prior result and add the ordinal value of
    // the current char 
    Result = (Result * 10) + c;
  }
  return Result;
}
void setup() {
   pinMode(dir_pin, OUTPUT);
   pinMode(step_pin, OUTPUT);
   pinMode(SLEEP, OUTPUT);
   Serial.begin(9600);
 
  lcd.begin(20, 4);
  lcd.setCursor(1,0);
  lcd.print("Magnetic Meter");
  lcd.setCursor(0,1);
  lcd.print("RaceAutoElectro");
  delay (2000);
 
   Serial.setTimeout(2000);
   digitalWrite(SLEEP, HIGH);  // Wake up EasyDriver
   delay(5);  // Wait for EasyDriver wake up
   
 



   while (!digitalRead(ZERO_POS)){
    digitalWrite(dir_pin,HIGH);
    digitalWrite(step_pin, HIGH);
    delay(1);
    digitalWrite(step_pin, LOW);
    delay(1);   //delay for reference point - BACK
    
   }
   
    while (digitalRead(ZERO_POS)){
    digitalWrite(dir_pin,LOW);
    digitalWrite(step_pin, HIGH);
    delay(1);
    digitalWrite(step_pin, LOW);
    delay(1);   //delay for reference point - FORWARD    ---0 REFERENCE---
    
   }
}
 
void loop() {
  incomingLen = Serial.available();
  if (incomingLen==0)
  {
    // if there is no data in serial input buffer, let's sleep
    // for a while
    delay(100);
  }
  else
  {
    // read the data in serial input buffer
    readLen = Serial.readBytes(buffer, incomingLen);
    if (readLen!=0)
    {
      // convert the ascii string number into correct binary form
      targetPos=(strToInt(buffer, readLen)-2550)*1.333;
      //targetPos=strToInt(buffer, readLen)*1.333;
      // notify sender of the entered value
      Serial.print("Entered value: ");
      Serial.println((targetPos+2550)/1.333);
       lcd.clear();
      lcd.print ("value:");
      lcd.setCursor(6,0);
      lcd.print ((targetPos+2550)/1.3333, DEC);

     
     
      // Print extra empty line
      Serial.println("");
    }
    else {
      Serial.println("empty");
    }
  }


  
    
  while (targetPos>0 && targetPos<MAX_STEPS && !digitalRead(ZERO_POS) && currentPos!=targetPos) {
    if (currentPos>targetPos) {
      
      digitalWrite(dir_pin, HIGH);  // (HIGH = anti-clockwise / LOW = clockwise)
      digitalWrite(step_pin, HIGH);
     // delay(1);
      digitalWrite(step_pin, LOW);
      //delay(1);
      for (int i=0;i<DELAY_TIME;i++) analogRead(A6);
      currentPos--;
    }
    if (currentPos<targetPos) {
      digitalWrite(dir_pin, LOW);  // (HIGH = anti-clockwise / LOW = clockwise)
      digitalWrite(step_pin, HIGH);
      //delay(1);
      digitalWrite(step_pin, LOW);
     //delay(1);
     for (int i=0;i<DELAY_TIME;i++) analogRead(A6);
      currentPos++;
    }
  } 
 }
