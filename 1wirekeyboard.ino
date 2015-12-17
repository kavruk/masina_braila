#define ENTER_KEY 41
int keypressed = 0;
int keyboardPin = 7;    // Analog input pin that the keypad is attached to
int keyboardValue = 0;   // value read from the keyboard

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
   Serial.begin(9600);
}
 
void loop() 
{                           
                           
 readkeyboard(); //get the value of key being pressed "keypressed" i.e. 0-9
 
}



//read the keyboard routine
void readkeyboard(){
     keyboardValue = analogRead(keyboardPin); // read the value (0-1023)
   if (keyboardValue <25){ //minimum press threshold
      delay(30);           //30ms delay for debounde
      keyboardValue=0;
      for(char i=0;i<5;i++) //read 5 samples 
        keyboardValue+=analogRead(keyboardPin);
      keyboardValue=keyboardValue/5;  //calculate 5 sample average
   }
     
 if ((keyboardValue >70) && (keyboardValue < 85)){keypressed = 41;}
 if ((keyboardValue >145) && (keyboardValue < 156)){keypressed = 7;}
 if ((keyboardValue >220) && (keyboardValue < 228)){keypressed = 4;}
 if ((keyboardValue >280) && (keyboardValue < 290)){keypressed = 1;}
 if ((keyboardValue >350) && (keyboardValue < 360)){keypressed = 0;}
 if ((keyboardValue >420) && (keyboardValue < 430)){keypressed = 8;}
 if ((keyboardValue >490) && (keyboardValue < 500)){keypressed = 5;}
 if ((keyboardValue >570) && (keyboardValue < 580)){keypressed = 2;}
 if ((keyboardValue >660) && (keyboardValue < 670)){keypressed = 42;}
 if ((keyboardValue >750) && (keyboardValue < 770)){keypressed = 9;}
 if ((keyboardValue >870) && (keyboardValue < 890)){keypressed = 6;}
 if ((keyboardValue >1010) && (keyboardValue < 1024)){keypressed = 3;}
 
  
  //NOTE: the values used above are all halfway between the value obtained with each keypress in previous test sketch 
   
  while (analogRead(keyboardPin) > 25) { //wait until key no longer pressed
   }
   
  Serial.println(keypressed);
}
