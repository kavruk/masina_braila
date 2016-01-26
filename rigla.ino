//#include <EnableInterrupt.h>
#define CLOCK 0
#define MOSI 11
byte full = 0;
byte pak=0;
char sendBuffer[10];
volatile byte spiVal = 0;
void setup() {
  Serial.begin(9600);
  pinMode(CLOCK, INPUT);
  pinMode(MOSI, INPUT);
  attachInterrupt(CLOCK, read, FALLING);
}

void loop() {
  if (8==full){
    pak++;
    if ((6<pak)&&(16>pak)){
      sendBuffer[pak-5]=spiVal;
    }
    full=0;
    spiVal=0;
  }
  if (20==pak){
    for (int i=0;i<10;i++)
      Serial.print(sendBuffer[i]);
    Serial.println("N");
    Serial.println(atoi(sendBuffer));
    pak=0;
  }
}
void read() {
  if (digitalRead(MOSI))
    spiVal = (spiVal << 1) + 1;
  else
    spiVal = (spiVal << 1);
  full++;
}
