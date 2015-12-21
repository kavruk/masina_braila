#define MAX_NUMBER 25000
#define ENTER_KEY 41
#define DEL_KEY 42
#define NOT_PRESSED_THRESHOLD 25
#define NO_KEY_PRESSED 255
int keypressed = 0;
int keyboardPin = 7;    // Analog input pin that the keypad is attached to
int keyboardValue = 0;   // value read from the keyboard
int composedNumber = 0;
#define DELAY_TIME 1
#include <LiquidCrystal.h>
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
int currentPos = 0, targetPos = 0;
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
void setup() {
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.setCursor(1, 0);
  Serial.setTimeout(2000);
}
void readkeyboard() {
  keyboardValue = analogRead(keyboardPin); // read the value (0-1023)
  if (keyboardValue > NOT_PRESSED_THRESHOLD) { //minimum press threshold
    delay(30);           //30ms delay for debounde
    keyboardValue = 0;
    for (char i = 0; i < 5; i++) //read 5 samples
      keyboardValue += analogRead(keyboardPin);
    keyboardValue = keyboardValue / 5; //calculate 5 sample average
  }
  Serial.print(keyboardValue);
  //keypressed=NO_KEY_PRESSED;
  //  if ((keyboardValue >70) && (keyboardValue < 90)){keypressed = ENTER_KEY;}
  //  if ((keyboardValue >145) && (keyboardValue < 156)){keypressed = 7;}
  //  if ((keyboardValue >220) && (keyboardValue < 228)){keypressed = 4;}
  //  if ((keyboardValue >280) && (keyboardValue < 290)){keypressed = 1;}
  //  if ((keyboardValue >350) && (keyboardValue < 360)){keypressed = 0;}
  //  if ((keyboardValue >420) && (keyboardValue < 430)){keypressed = 8;}
  //  if ((keyboardValue >490) && (keyboardValue < 500)){keypressed = 5;}
  //  if ((keyboardValue >570) && (keyboardValue < 580)){keypressed = 2;}
  //  if ((keyboardValue >660) && (keyboardValue < 670)){keypressed = DEL_KEY;}
  //  if ((keyboardValue >750) && (keyboardValue < 770)){keypressed = 9;}
  //  if ((keyboardValue >870) && (keyboardValue < 890)){keypressed = 6;}
  //  if ((keyboardValue >1010) && (keyboardValue < 1024)){keypressed = 3;}
  //  Serial.println(keypressed);
  //  //if (255!=keypressed) {
  //NOTE: the values used above are all halfway between the value obtained with each keypress in previous test sketch
  //    if (analogRead(keyboardPin) > NOT_PRESSED_THRESHOLD){
  //      int temp = composedNumber;
  //      composedNumber*=10;
  //      composedNumber+=keypressed;
  //      if (composedNumber>MAX_NUMBER || composedNumber<0){
  //        composedNumber=temp;
  //        Serial.println("Error: Out of bounds!");
  //      }
  //      lcd.clear();
  //      lcd.print(composedNumber);
  //
  //      if (ENTER_KEY==keypressed)  {                //set the target to current number
  //        Serial.println("ENTER pressed");
  //      }
  //      if (DEL_KEY==keypressed)  {                //clear the current number
  //        Serial.println("DELETE pressed");
  //        composedNumber=0;
  //      }
  //    }
  //}
  while (analogRead(keyboardPin) > NOT_PRESSED_THRESHOLD) { //wait until key no longer pressed
  }

}
void loop() {
  incomingLen = Serial.available();
  readkeyboard(); //get the value of key being pressed "keypressed" i.e. 0-9
  if (incomingLen == 0)
  {
    // if there is no data in serial input buffer, let's sleep
    // for a while
    delay(100);
  }
  else
  {
    // read the data in serial input buffer
    readLen = Serial.readBytes(buffer, incomingLen);
    if (readLen != 0)
    {
      // convert the ascii string number into correct binary form
      targetPos = (strToInt(buffer, readLen) - 2550) * 1.3333;
      // notify sender of the entered value
      Serial.print("Entered value: ");
      Serial.println(targetPos / 1.333 + 2550);
      lcd.clear();
      lcd.print ("value:");
      lcd.setCursor(6, 0);
      lcd.print (targetPos / 1.333 + 2550, DEC);
      // Print extra empty line
      Serial.println("");
    }
    Serial.println("empty");
  }


}
