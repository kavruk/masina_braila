//#include <EnableInterrupt.h>
#define CLOCK 0
//#define MOSI 11
//byte full = 0;
volatile byte spiVal = 0;
void setup() {
  Serial.begin(9600);
  pinMode(CLOCK, INPUT);
  pinMode(MOSI, INPUT);
  attachInterrupt(digitalPinToInterrupt(CLOCK), read, CHANGE);
}

void loop() {
    Serial.println(spiVal);
}

void read() {
  //spiVal |= (spiVal << 1) + digitalRead(MOSI);
  spiVal=17;
}
