#include <SPI.h>

// CR95HF: send, poll, read, reset
#define   SEND            0x00
#define   POLL            0x03
#define   READ            0x02
#define   RESET           0x01

// CR95HF: echo
//#define   ECHO            0x55

//Pin definition
const int SPI_SS = 10;
const int IRQ_IN = 8;
const int SPI_MISO = 12;
const int SPI_MOSI = 11;
const int SSI_0 = 9;
const int SPI_SCK = 13;
const int IRQ_OUT = 2;

void PowerUp(){
  // Envoie les signaux pour demarrer la carte
  digitalWrite(9,HIGH);
  delayMicroseconds(100);
  digitalWrite(8,LOW);
  delayMicroseconds(50);
  digitalWrite(8,HIGH);  
}

void ECHO(){
  //SEND to CR95HF
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(SEND);
  SPI.transfer(0x55);
  digitalWrite(SPI_SS, HIGH);

  //READ from CR95HF
  byte r = 0;
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(READ);
  r = SPI.transfer(0x00);
  digitalWrite(SPI_SS, HIGH);
  Serial.print("r = ");
  Serial.print(r, HEX);
  Serial.println();
  
}

void IDN(){
  //SEND to CR95HF
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(SEND);
  SPI.transfer(0x01);
  SPI.transfer(0x00);
  digitalWrite(SPI_SS, HIGH);

  //POLL CR95HF
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(POLL);
  byte flag = 0x00; // Initialise la boucle
  //Test du bit n°3 de MISO
  while(bitRead(flag,3)!=1){
    flag = SPI.transfer(POLL);
    delay(100);
    }
  digitalWrite(SPI_SS, HIGH);

  //READ from CR95HF
  Serial.print("IDN = ");
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(READ);
  byte resp = SPI.transfer(0x00);
  byte len = SPI.transfer(0x00);
  for (int i = 0; i <= len; i++) {
    byte data = SPI.transfer(0x00);
    Serial.print(char(data));
  }
    Serial.println();
  digitalWrite(SPI_SS, HIGH);
  
}

void setup() {
  Serial.begin(500000);

  pinMode(SPI_SS, OUTPUT);
  pinMode(IRQ_IN, OUTPUT);

  digitalWrite(IRQ_IN, HIGH); // wakeup
  digitalWrite(SPI_SS, HIGH);  // slave select, low = ready to write/read

  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

  PowerUp();
  ECHO();
  IDN();
}

void loop() {

  //ECHO();
  
}
