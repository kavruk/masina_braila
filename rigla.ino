#include <SPI.h>
#define DATAOUT 11//MOSI
#define DATAIN  12//MISO 
#define SPICLOCK  13//sck
#define SLAVESELECT 10//ss
byte eeprom_input_data = 0;
byte clr;
int address = 0;
char spi_transfer(volatile char data)
{
  SPDR = data;                    // Start the transmission
  while (!(SPSR & (1 << SPIF)))   // Wait the end of the transmission
  {
  };
  return SPDR;                    // return the received byte
}
void setup() {
  Serial.begin(9600);
  pinMode(DATAOUT, OUTPUT);
  pinMode(DATAIN, INPUT);
  pinMode(SPICLOCK, OUTPUT);
  pinMode(SLAVESELECT, OUTPUT);
  digitalWrite(SLAVESELECT, LOW); //disable device
  // SPCR = 01010000
  //interrupt disabled,spi enabled,msb 1st,master,clk low when idle,
  //sample on leading edge of clk,system clock/4 rate (fastest)
  SPCR = (1 << SPE);
  clr = SPSR;
  clr = SPDR;
  delay(10);
}
void loop() {
  // put your main code here, to run repeatedly:
  int data = spi_transfer(0xFF);
  Serial.println(data);
  delay(10);
git}
