#define MAX_NUMBER 25000
#define ENTER_KEY 41
#define DEL_KEY 42
#define NOT_PRESSED_THRESHOLD 25
#define NO_KEY_PRESSED 255
#define SERIAL_ENABLED 1
#define SAMPLES_NUMBER 5

int keypressed = 0;
int keyboardPin = 7;    // Analog input pin that the keypad is attached to
int keyboardValue = 0;   // value read from the keyboard
int composedNumber[4];
int numberOfMeasurement=0;
#include <LiquidCrystal.h>
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
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
    numberOfMeasurement++;
	if (4==numberOfMeasurement){
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
    lcd.setCursor(0,numberOfMeasurement);
    lcd.print("       ");
    lcd.setCursor(0,numberOfMeasurement);
    composedNumber[numberOfMeasurement]=0;
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
      lcd.setCursor(0, numberOfMeasurement);
      lcd.print(composedNumber[numberOfMeasurement]);
      // Print extra empty line
      Serial.println("");
      numberOfMeasurement++;
      if (4==numberOfMeasurement){
        numberOfMeasurement=0;
        for(int i=0;i<4;i++){
          composedNumber[i]=0;      //clear all stored readings
        }
      }
    }
    Serial.println("empty");
  }
#endif
}
