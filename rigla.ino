//https://dl.bintray.com/greygnome/generic/enableinterrupt-0.9.4.zip
#include <EnableInterrupt.h>
#define CLOCK 13
#define MOSI 11
byte full = 0;
byte spiVal = 0;
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
