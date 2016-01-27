#define CLOCK 0
#define MOSI 11
#define DEBUG 1
#define EXTENSION 13
byte full = 0;
byte pak = 0;
char sendBuffer[10];
volatile byte spiVal = 0;
void read() {
  spiVal = (spiVal << 1) + digitalRead(MOSI);
  full++;
}
void setup() {
  Serial.begin(115200);
  pinMode(CLOCK, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(EXTENSION, INPUT);
  attachInterrupt(CLOCK, read, FALLING);
}
void loop() {
  if (8 == full) {
    pak++;
    full = 0;
#if DEBUG
    Serial.print(pak);
    Serial.print(":");
    Serial.print(spiVal);
    Serial.print(" ");
#endif
    if ((pak > 6) && (pak < 17)) {
      sendBuffer[pak - 7] = spiVal;
    }
  }
  if (20 == pak) {
    //detachInterrupt(CLOCK);
    if (142 == sendBuffer[19])
      Serial.println(atoi(sendBuffer) + 10000 * digitalRead(EXTENSION));
    pak = 0;
  }
}
