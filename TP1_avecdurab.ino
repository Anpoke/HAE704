#include <SPI.h>

SPISettings  mySettings(1000000, MSBFIRST, SPI_MODE0); //ATTENTION AU MODE
void PowerUp(){
  digitalWrite(8,HIGH); 
  digitalWrite(9,HIGH);
  delayMicroseconds(110);
  digitalWrite(8,LOW);
  delayMicroseconds(15);
  digitalWrite(8,HIGH);
  delay(12);  
  digitalWrite(10,HIGH);
}

void PinSetup(){
  pinMode(13,INPUT);//SPI_SCK
  pinMode(12,OUTPUT);//SPI_MISO
  pinMode(11,INPUT);//SPI_MOSI
  pinMode(10,OUTPUT); //SPI_SS
  pinMode(9,OUTPUT); //SSI_0 -> Interface Pin
  pinMode(8,OUTPUT); //IRQ_IN
  pinMode(2,OUTPUT); //IRQ_OUT
}

void CMD_IDN(){
  //Envoie la commande IDN
  SPI.beginTransaction(mySettings);
  digitalWrite(10,LOW);
  SPI.transfer(0x00);
  SPI.transfer(0x01);
  SPI.transfer(0x00);
  digitalWrite(10,HIGH);
  SPI.endTransaction();
}

void Polling(){
  SPI.beginTransaction(mySettings);
  digitalWrite(10,LOW);
  SPI.transfer(0x03);
  byte flag = 0x00;

  //Test du bit n°3 de MISO
  while(bitRead(flag,3)!=1){
    flag = SPI.transfer(0x03);
  }
  
  SPI.endTransaction();
  digitalWrite(10,HIGH); 
}
void setup() {
  //Good to know
     //Serial.begin(speed, config) 
     //SPI.begin
     
  
  
}



void loop() {


  
}
