//#include <EnableInterrupt.h>
#define CLOCK 0
#define MOSI 11
byte full = 0;
volatile byte spiVal = -1;
void setup() {
  Serial.begin(9600);
  pinMode(CLOCK, INPUT);
  pinMode(MOSI, INPUT);
  attachInterrupt(CLOCK, read, FALLING);
}

void loop() {
  if (7==full){
    Serial.println(spiVal);
    full=0;
  }
}

void read() {
  spiVal |= (spiVal << 1) + digitalRead(MOSI);
  full++;
}
