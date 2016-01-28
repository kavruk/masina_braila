#define CLOCK 0
#define MOSI 11
#define DEBUG 0
#define EXTENSION 13
byte full = 0;
byte pak = 0;
char sendBuffer[10];
volatile byte spiVal = 0;
byte send = 0;
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
    send++;
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
#if DEBUG
    Serial.println();
#endif
    if (send == 2) {

      Serial.println(atoi(sendBuffer) + 10000 * digitalRead(EXTENSION));
      send = 0;
    }
    pak = 0;
  }
}
