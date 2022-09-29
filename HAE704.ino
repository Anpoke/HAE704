#include <SPI.h>
//Pin definition
#define SPI_SCK 13
#define SPI_MISO 12
#define SPI_MOSI 11
#define SPI_SS 10
#define SSI_0 9
#define IRQ_IN 8
#define IRQ_OUT 2

//Control byte definition
#define Send 0x00
#define Poll 0x03
#define ReadData 0x02
#define Reset 0x01

//Command definition
#define IDN 0x01
#define ProtocolSelect 0x02
#define SendRecv 0x04
#define IDLE 0x07
#define RDREG 0x08
#define WRREG 0x09
#define BaudRate 0x0A
#define Echo 0x55

SPISettings  mySettings(1000000, MSBFIRST, SPI_MODE0); //ATTENTION AU MODE

void PowerUp(){
  //Envoie les signaux nécessaires au démarrage de la carte
  digitalWrite(IRQ_IN,HIGH); 
  digitalWrite(SSI_0,HIGH);
  delayMicroseconds(110);
  digitalWrite(IRQ_IN,LOW);
  delayMicroseconds(15);
  digitalWrite(IRQ_IN,HIGH);
  delay(11);  
 // digitalWrite(SPI_SS,HIGH);
}

void PinSetup(){
  pinMode(SPI_SCK,INPUT);
  pinMode(SPI_MISO,OUTPUT);
  pinMode(SPI_MOSI,INPUT);
  pinMode(SPI_SS,OUTPUT);
  pinMode(SSI_0,OUTPUT);
  pinMode(IRQ_IN,OUTPUT);
  pinMode(IRQ_OUT,OUTPUT);
}

//attribuer des noms aux octets pour ne pas se perdre
void CMD_IDN(){
  //Envoie la commande IDN
  SPI.beginTransaction(mySettings);
  digitalWrite(SPI_SS,LOW);
  delay(10);
  SPI.transfer(Send);
  SPI.transfer(IDN);
  SPI.transfer(0x00); //LEN for IDN
  digitalWrite(SPI_SS,HIGH);
  SPI.endTransaction();
}

void Polling(){
  //Envoie la commande Polling
  SPI.beginTransaction(mySettings);
  //digitalWrite(10,LOW);
  SPI.transfer(Poll);
  byte flag = 0x00; // Initialise la boucle

  //Test du bit n°3 de MISO
  while(bitRead(flag,3)!=1){
    flag = SPI.transfer(Poll);
    delay(100); //Evite de harceler le module
  }
  
  SPI.endTransaction();
  //digitalWrite(10,HIGH); 
}

void Reading(){
  //Envoie la commande reading
  byte Resp = 0x00;
  byte LEN = 0x00;
  digitalWrite(SPI_SS,LOW);
  SPI.beginTransaction(mySettings);

  SPI.transfer(ReadData);
  SPI.transfer(0x10);
  Serial.print("Resp is ");
  Resp=Serial.print(Resp,BIN);
  SPI.transfer(0x10);
  Serial.print(" LEN is ");
  LEN=Serial.print(LEN, BIN);
  delay(10);
  SPI.endTransaction();
  digitalWrite(SPI_SS,HIGH);
}

void ECHO(){
  byte Echo1=0x00;
  digitalWrite(SPI_SS,LOW);
  SPI.beginTransaction(mySettings);
  SPI.transfer(Send);
  Echo1=SPI.transfer(Echo);
  //Serial.print("Echo is ");
  //Serial.print(Echo1, BIN);
  SPI.endTransaction();
  digitalWrite(SPI_SS,HIGH);
  delay(10);
}

void setup() {
  //Good to know
     //Serial.begin(9600) 
     //SPI.begin

Serial.begin(9600);
PowerUp();

SPI.begin();
//pinMode(SPI_MISO,OUTPUT);
//pinMode(SSI_0,OUTPUT);
//pinMode(IRQ_IN,OUTPUT);
//pinMode(IRQ_OUT,OUTPUT);
SPISettings  mySettings(1500000, MSBFIRST, SPI_MODE3);

  digitalWrite(SPI_SS,LOW);
  SPI.beginTransaction(mySettings);
  SPI.transfer(Send);
  SPI.transfer(Echo);
   SPI.endTransaction();
  digitalWrite(SPI_SS,HIGH);

  digitalWrite(SPI_SS,LOW);
  SPI.beginTransaction(mySettings);
  SPI.transfer(ReadData);
  byte r =SPI.transfer(0x00);
  Serial.print(" R EGAL ");
  Serial.print(r, HEX);
  SPI.endTransaction();
  digitalWrite(SPI_SS,HIGH);
  delay(10);

}



void loop() {


  
}
