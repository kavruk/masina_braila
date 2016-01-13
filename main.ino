//keyboard
#define MAX_NUMBER 25000
#define MIN_NUMBER 2560
#define ENTER_KEY 41
#define DEL_KEY 42
#define NOT_PRESSED_THRESHOLD 25
#define NO_KEY_PRESSED 255
#define SERIAL_ENABLED 1
#define ZERO_POS 2//pin punct 0 
#define CLEAR_20CMc
#define SAMPLES_NUMBER 5
int keypressed = 0;
int keyboardPin = 0;    // Analog input pin that the keypad is attached to 0 la test ... 7 pe masina 
int keyboardValue = 0;   // value read from the keyboard
int composedNumber[4];
int numberOfMeasurement=0;
#define CUT_ENGAGED 3
#define CUT_EXECUTED 4
#define CLEAR_20CM 2000
//display
#include <LiquidCrystal.h>
LiquidCrystal lcd(11, 12, A2, A3, A4, A5); /// 11, 12 pt test  a1 , a2 pe masina 
//motor
#define step_pin 9 // Pin 3 connected to Steps pin on EasyDriver
#define dir_pin 10   // Pin 2 connected to Direction pin
#define SLEEP 7     // Pin 7 connected to SLEEP pin
#define DELAY_TIME 8   // 1 pe masina .....8 pt test 
int currentPos=0;

//serial
#if SERIAL_ENABLED
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
  for (int i = 0; i < ALen; i++)
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
#endif
void setup() {
  pinMode(dir_pin, OUTPUT);
  pinMode(step_pin, OUTPUT);
  pinMode(SLEEP, OUTPUT);
  digitalWrite(SLEEP, HIGH);  // Wake up EasyDriver
  delay(5);  // Wait for EasyDriver wake up
  while (!digitalRead(ZERO_POS)){      //MOVE to zero
    digitalWrite(dir_pin,HIGH);
    digitalWrite(step_pin, HIGH);
    delay(1);
    digitalWrite(step_pin, LOW);
    delay(1);   //delay for reference point - BACK
   }
   while (digitalRead(ZERO_POS)){     //MOVE forward until not touching 0 point any more
    digitalWrite(dir_pin,LOW);
    digitalWrite(step_pin, HIGH);
    delay(1);
    digitalWrite(step_pin, LOW);
    delay(1);   //delay for reference point - FORWARD    ---0 REFERENCE---
   }
#if SERIAL_ENABLED
  Serial.begin(9600);
  Serial.setTimeout(2000);
#endif
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  lcd.blink();
  for (int i=0;i<4;i++)
    composedNumber[i]=0;
}
void readkeyboard() {
  keyboardValue = analogRead(keyboardPin); // read the value (0-1023)
  if (keyboardValue > NOT_PRESSED_THRESHOLD) { //minimum press threshold
    delay(30);           //30ms delay for debounce
    keyboardValue = 0;
    for (char i = 0; i < SAMPLES_NUMBER; i++) //read 5 samples
      keyboardValue += analogRead(keyboardPin);
    keyboardValue = keyboardValue / SAMPLES_NUMBER; //calculate 5 sample average
  }
  keypressed=NO_KEY_PRESSED;
  if ((keyboardValue >70) && (keyboardValue < 90)){keypressed = ENTER_KEY;}
  if ((keyboardValue >145) && (keyboardValue < 156)){keypressed = 7;}
  if ((keyboardValue >220) && (keyboardValue < 228)){keypressed = 4;}
  if ((keyboardValue >280) && (keyboardValue < 290)){keypressed = 1;}
  if ((keyboardValue >350) && (keyboardValue < 360)){keypressed = 0;}
  if ((keyboardValue >420) && (keyboardValue < 430)){keypressed = 8;}
  if ((keyboardValue >490) && (keyboardValue < 500)){keypressed = 5;}
  if ((keyboardValue >570) && (keyboardValue < 582)){keypressed = 2;}
  if ((keyboardValue >660) && (keyboardValue < 672)){keypressed = DEL_KEY;}
  if ((keyboardValue >750) && (keyboardValue < 770)){keypressed = 9;}
  if ((keyboardValue >870) && (keyboardValue < 890)){keypressed = 6;}
  if ((keyboardValue >1010) && (keyboardValue < 1024)){keypressed = 3;}
  Serial.println(keyboardValue);
  if (keypressed>=0&&keypressed<=9) {     //create multi char number from key presses
    int temp = composedNumber[numberOfMeasurement];
    composedNumber[numberOfMeasurement]*=10;
    composedNumber[numberOfMeasurement]+=keypressed;
    if (composedNumber[numberOfMeasurement]>MAX_NUMBER || composedNumber[numberOfMeasurement]<0){   //oare trebuie bagata si valoare minima??? 20cm?
      composedNumber[numberOfMeasurement]=temp;
#if SERIAL_ENABLED
      Serial.println("Error: Out of bounds!");
#endif
     }
     lcd.setCursor(0,numberOfMeasurement);
     lcd.print(composedNumber[numberOfMeasurement]);
#if SERIAL_ENABLED
      Serial.println(composedNumber[numberOfMeasurement]);
#endif
  }
  if (ENTER_KEY==keypressed){                //set the target to current number
#if SERIAL_ENABLED
    Serial.println("ENTER pressed");
#endif
  if (composedNumber[numberOfMeasurement]<MIN_NUMBER){
#if SERIAL_ENABLED
        Serial.print("Error: Number smaller than ");
        Serial.println(MIN_NUMBER);
#endif
        lcd.setCursor(0,numberOfMeasurement);
        lcd.print("Numar prea mic");
        delay(500);
        deleteRow();
        if (numberOfMeasurement>=0)
           numberOfMeasurement--;  
     }
  numberOfMeasurement++;
#if SERIAL_ENABLED
    Serial.print("number of measurement = ");
    Serial.println(numberOfMeasurement);
#endif
  if (5==numberOfMeasurement){            
        numberOfMeasurement=0;
        for(int i=0;i<4;i++){
          composedNumber[i]=0;      //clear all stored readings
          lcd.clear();
         lcd.setCursor(0,0);
        }
      }
    lcd.setCursor(0, numberOfMeasurement);
  }
  if (DEL_KEY==keypressed)  {                //clear the current number
#if SERIAL_ENABLED
    Serial.println("DELETE pressed");
#endif
    deleteRow();
    if (numberOfMeasurement>0)
      numberOfMeasurement--;
  }
  while (analogRead(keyboardPin) > NOT_PRESSED_THRESHOLD) { //wait until key no longer pressed
  }
}
void loop() {
  readkeyboard(); //get the value of key being pressed "keypressed" i.e. 0-9
#if SERIAL_ENABLED
  incomingLen = Serial.available();
  if (incomingLen == 0)
  {
    // if there is no data in serial input buffer, let's sleep for a while
    delay(100);
  }
  else {
    // read the data in serial input buffer
    readLen = Serial.readBytes(buffer, incomingLen);
    if (readLen != 0){
      // convert the ascii string number into correct binary form
      composedNumber[numberOfMeasurement] = strToInt(buffer, readLen);
      // notify sender of the entered value
      Serial.print("Entered value: ");
      Serial.println(composedNumber[numberOfMeasurement]);
      // Print extra empty line
      Serial.println("");
      numberOfMeasurement++;
      lcd.setCursor(0, numberOfMeasurement);
      lcd.print(composedNumber[numberOfMeasurement]);
    }
  }
#endif    
      //move motor
  if (4==numberOfMeasurement){
#if SERIAL_ENABLED        
    Serial.println("4 measurements entered");
#endif        
    for (int i=0;i<4;i++) {
      while (composedNumber[i]>0 && composedNumber[i]<MAX_NUMBER && !digitalRead(ZERO_POS) && currentPos!=composedNumber[i]) {
#if SERIAL_ENABLED
        Serial.print(currentPos);
        Serial.print(":");
        Serial.println(composedNumber[i]);
        Serial.println(digitalRead(CUT_ENGAGED));
        Serial.println(digitalRead(CUT_EXECUTED));
#endif 
        if (currentPos>composedNumber[i]) {
          digitalWrite(dir_pin, HIGH);  // (HIGH = anti-clockwise / LOW = clockwise)
          digitalWrite(step_pin, HIGH);
          // delay(1);
          digitalWrite(step_pin, LOW);
          //delay(1);
          for (int i=0;i<DELAY_TIME;i++) analogRead(A6);
            currentPos--;
        }
        if (currentPos<composedNumber[i]) {
          digitalWrite(dir_pin, LOW);  // (HIGH = anti-clockwise / LOW = clockwise)
          digitalWrite(step_pin, HIGH);
          //delay(1);
          digitalWrite(step_pin, LOW);
          //delay(1);
          for (int i=0;i<DELAY_TIME;i++) 
            analogRead(A6);
          currentPos++;
        }
      }
      while(digitalRead(CUT_ENGAGED)==1);
      while(digitalRead(CUT_EXECUTED)==0);
      while(digitalRead(CUT_ENGAGED)==1);
      delay(5000);
      //move forward 20cm
      digitalWrite(dir_pin, LOW);  // (HIGH = anti-clockwise / LOW = clockwise)
      for (int i=0;i<CLEAR_20CM;i++){
        digitalWrite(step_pin, HIGH);
        delay(1);
        digitalWrite(step_pin, LOW);
        delay(1);
      }   
      lcd.setCursor(i,7);
      lcd.print("ia bagheta");
      delay(5000);
      lcd.setCursor(i,7);
      lcd.print("terminat  ");
    }
    numberOfMeasurement=0;
    for(int i=0;i<4;i++){
      composedNumber[i]=0;      //clear all stored readings
    }
  }
}
void deleteRow(){
  lcd.setCursor(0,numberOfMeasurement);
  lcd.print("              ");
  lcd.setCursor(0,numberOfMeasurement);
  composedNumber[numberOfMeasurement]=0;
}
