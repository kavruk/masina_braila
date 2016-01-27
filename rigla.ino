#define CLOCK 0
#define MOSI 11
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
  attachInterrupt(CLOCK, read, FALLING);
}

void loop() {
  if (8 == full) {
    pak++;
    full = 0;
    if ((pak > 6) && (pak < 17)) {
      sendBuffer[pak - 7] = spiVal;
      Serial.print(pak - 7);
      Serial.print(":");
      Serial.print(spiVal, HEX);
      Serial.print(":");
      Serial.print(sendBuffer[pak - 7]);
      Serial.print(" ");
    }
  }
  if (20 == pak) {
    /*for (int i=0;i<10;i++)
      Serial.print(sendBuffer[i]);
      Serial.println("N");
      Serial.println(atoi(sendBuffer));
      pak=0;*/
    Serial.println(atoi(sendBuffer));
    Serial.println();
    pak = 0;
  }
}
