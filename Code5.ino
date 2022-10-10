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
  digitalWrite(SSI_0,HIGH);
  delayMicroseconds(100);
  digitalWrite(8,LOW);
  delayMicroseconds(50);
  digitalWrite(8,HIGH);
  delay(1000);
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
  Serial.print("Echo = ");
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
  POLLING();
  Serial.print("IDN = ");
  READING();
}

void PROTOCOLSELECT(){
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(SEND);
  SPI.transfer(0x02);
  SPI.transfer(0x02); // length
  SPI.transfer(0x02); // Protocol ISO/IEC 14443-A
  SPI.transfer(0x00); // Transmission et réception à 106 Kbps
  digitalWrite(SPI_SS, HIGH);
  POLLING();
  READING();
}


void POLLING(){ //POLL CR95HF
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(POLL);
  byte flag = 0x00; // Initialise la boucle
  //Test du bit n°3 de MISO
  while(bitRead(flag,3)!=1){
    flag = SPI.transfer(POLL);
    delay(100); //Evite de harceler le module
    //Serial.print(bitRead(flag,3)); //Permet de vérifier qu'on consulte la bonne valeur
    //Serial.println();
    }
  digitalWrite(SPI_SS, HIGH);
}

void READING(){
//READ from CR95HF
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(READ);
  byte resp = SPI.transfer(0x00);
  byte len = SPI.transfer(0x00);
  for (int i = 0; i < len; i++) { //Lecture et affichage successif des données
    byte data = SPI.transfer(0x00);
    Serial.print(char(data));
  }
    Serial.println();
  digitalWrite(SPI_SS, HIGH);
  
}

void setup() {
  Serial.begin(500000); //Attention à faire coincider le baudrate

  pinMode(SPI_SS, OUTPUT);
  pinMode(IRQ_IN, OUTPUT);

  digitalWrite(IRQ_IN, HIGH); // wakeup
  digitalWrite(SPI_SS, HIGH);  // slave select, low = ready to write/read

  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

  PowerUp();
  ECHO();
  IDN();
  PROTOCOLSELECT();

}

void loop() {

  //ECHO();
  //IDN();
}
