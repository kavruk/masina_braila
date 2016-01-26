//#include <EnableInterrupt.h>
#define CLOCK 0
#define MOSI 11
byte full = 0;
byte pak=0;
volatile byte spiVal = 0;
void setup() {
  Serial.begin(9600);
  pinMode(CLOCK, INPUT);
  pinMode(MOSI, INPUT);
  attachInterrupt(CLOCK, read, FALLING);
}

void loop() {
  if (8==full){
    Serial.print(spiVal,HEX);
    Serial.print(" ");
    full=0;
    spiVal=0;
    pak++;
  }
  if (20==pak){
    Serial.println();
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
