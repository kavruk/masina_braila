#include <SPI.h>
#define DATAOUT 11//MOSI
#define DATAIN  12//MISO 
#define SPICLOCK  13//sck
#define SLAVESELECT 10//ss

int readStuff(void) {
  SPI.beginTransaction(SPISettings(12000000, MSBFIRST, SPI_MODE0));  // gain control of SPI bus
  digitalWrite(SLAVESELECT, HIGH);         // assert chip select
  SPI.transfer(0x74);            // send 16 bit command
  SPI.transfer(0xA2);
  byte b1 = SPI.transfer(0);     // read 16 bits of data
  byte b2 = SPI.transfer(0);
  digitalWrite(10, HIGH);        // deassert chip select
  SPI.endTransaction();          // release the SPI bus
  return (int16_t)((b1 << 8) | b2);
}
void setup() {
  Serial.begin(9600);
  pinMode(DATAOUT, OUTPUT);
  pinMode(DATAIN, INPUT);
  pinMode(SPICLOCK, OUTPUT);
  pinMode(SLAVESELECT, OUTPUT);
  SPCR = (1 << SPE);
  
}
void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(readStuff());
}
