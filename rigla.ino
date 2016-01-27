#define CLOCK 0
#define MOSI 11
byte full = 0;
byte pak=0;
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
  if (8==full){
    pak++;
    full=0;
    if ((pak>4)&&(pak<15)){
      sendBuffer[pak-5]=spiVal;
    }
    Serial.print(sendBuffer[pak-5],HEX);
    //Serial.println(pak);
    //Serial.print(pak);
    Serial.print(" ");
    
  }
  if (20==pak){
    /*for (int i=0;i<10;i++)
      Serial.print(sendBuffer[i]);
    Serial.println("N");
    Serial.println(atoi(sendBuffer));
    pak=0;*/
    Serial.println();
    pak=0;
  }
}
