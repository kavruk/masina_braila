#define CLOCK 0
#define MOSI 11
#define DEBUG 0
#define EXTENSION 1
byte full = 0;
byte pak = 0;
char sendBuffer[10];
volatile byte spiVal = 0;
void read() {
  spiVal = (spiVal << 1) + digitalRead(MOSI);
  full++;
}
void setup() {
  Serial.begin(9600);
  pinMode(CLOCK, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(EXTENSION,INPUT);
  attachInterrupt(CLOCK, read, FALLING);
}

void loop() {
  if (8 == full) {
    pak++;
    full = 0;
    if ((pak > 6) && (pak < 17)) {
      sendBuffer[pak - 7] = spiVal;
#ifdef DEBUG
      Serial.print(pak - 7);
      Serial.print(":");
      Serial.print(spiVal, HEX);
      Serial.print(":");
      Serial.print(sendBuffer[pak - 7]);
      Serial.print(" ");
#endif
    }
  }
  if (20 == pak) {
    detachInterrupt(CLOCK);
    Serial.println(atoi(sendBuffer)+10000*digitalRead(EXTENSION));
    pak = 0;
  }

}
