#include <EnableInterrupt.h>
#define CLOCK 0
#define MOSI 11
byte full = 0;
volatile byte spiVal = 0;
void setup() {
  Serial.begin(115200);
  pinMode(CLOCK, INPUT);
  pinMode(MOSI, INPUT);
  enableInterrupt(digitalPinToInterrupt(CLOCK), read, CHANGE);
}

void loop() {
  //if (full != 0) 
  {
    full = 0;
    Serial.println(spiVal);
  }
}

void read() {
  //spiVal |= (spiVal << 1) + digitalRead(MOSI);
  spiVal=17;
  full++;
}
