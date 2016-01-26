#define CLOCK 13
#define MOSI 11
byte full=0;
byte spiVal=0;
void setup() {
    pinMode(CLOCK, INPUT);
    pinMode(MOSI,INPUT);
    attachInterrupt(digitalPinToInterrupt(CLOCK), read, FALLING);
}

void loop() {
    if (full==7){
      full=0;
      Serial.println(spiVal);
    }
}

void read() {
    spiVal|=(spiVal<<1)+digitalRead(MOSI);
    full++;
}
