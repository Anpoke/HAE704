#include <SPI.h>

// CR95HF: send, poll, read, reset
#define   SEND            0x00
#define   POLL            0x03
#define   READ            0x02
#define   RESET           0x01

//Pin definition
const int SPI_SS = 10;
const int IRQ_IN = 8;
//const int SPI_MISO = 12;
//const int SPI_MOSI = 11;
const int SSI_0 = 9;
//const int SPI_SCK = 13;
//const int IRQ_OUT = 2;

//Commands
const byte IDN = 0x01;
const byte ProtocolSelect = 0x02;
const byte Echo = 0x55;

void PowerUp(){ // Envoie les signaux pour demarrer la carte
  digitalWrite(SSI_0,HIGH);
  delayMicroseconds(100);
  digitalWrite(IRQ_IN,LOW);
  delayMicroseconds(15);
  digitalWrite(IRQ_IN,HIGH);
  delay(10);
}

void POLLING(){ //POLL CR95HF
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(POLL);
  byte flag = 0x00; // Initialise la boucle
  //Test du bit n°3 de MISO
  while(bitRead(flag,3)!=1){
    flag = SPI.transfer(POLL);
    delay(10); //Evite de harceler le module, optionnel
    //Serial.print(bitRead(flag,3)); //Permet de vérifier qu'on consulte la bonne valeur
    //Serial.println();
    }
  digitalWrite(SPI_SS, HIGH);
  //Serial.print("Polled");
  //Serial.println();
}
byte resp=0x00;
void READING(byte control=0xFF){ //READ from CR95HF
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(READ);
  resp = SPI.transfer(0x00);
  if(control==0x02 && resp==0x00){ // On souhaite confirmer que le protocole a été selectionné correctement
  Serial.print("Successful protocol selection");
  Serial.println();
  }
  if (control==0x04){
    //Serial.print(resp,HEX);
    switch(resp){
    case 0x87:
    Serial.print("No tag detected");
    Serial.println();
    break;

    case 0x80:
    Serial.print("Tag detected");
    Serial.println();
    //Serial.print("Tag = ");
    break;

    default:
    Serial.print("Resp = ");
    Serial.print(resp,HEX);
    Serial.println();
    break;
    }    
  }
  byte len = SPI.transfer(0x00);
  //Serial.print(len);
  for (int i = 0; i < len; i++) { //Lecture et affichage successif des données
    byte data = SPI.transfer(0x00);
    if(control==IDN){
    Serial.print(char(data));
    }
    else{
    Serial.print(data,HEX);
    Serial.print(" ");
    }
  }
    Serial.println();
  digitalWrite(SPI_SS, HIGH);
}


void SENDING(byte command, byte length){
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(SEND);

  if (command == Echo){
  SPI.transfer(command);
  digitalWrite(SPI_SS, HIGH);
  //READ Echo from CR95HF
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(READ);
  byte r = SPI.transfer(0x00);
  digitalWrite(SPI_SS, HIGH);
  Serial.print("Echo = ");
  Serial.print(r, HEX);
  Serial.println();
  }

  else{
    switch(command){
      case IDN:
      SPI.transfer(command);
      SPI.transfer(length);
      Serial.print("IDN = ");
      break;

      case ProtocolSelect:
      SPI.transfer(command);
      SPI.transfer(length);
      byte prtcl = SPI.transfer(0x02); // Protocol ISO/IEC 14443-A
      byte prtcl2 = SPI.transfer(0x00); // Transmission et réception à 106 Kbps
      break;
    }
  }
  digitalWrite(SPI_SS, HIGH);
  if (command != Echo){
  POLLING();
  READING(command);
  }
}

int l=0;

void REQUESTA(){
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(SEND);
  SPI.transfer(0x04);
  SPI.transfer(0x02);
  SPI.transfer(0x26);
  SPI.transfer(0x07);
  digitalWrite(SPI_SS, HIGH);
  POLLING();
  READING(0x04);
}

void WAKEUP(){
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(SEND);
  SPI.transfer(0x04);
  SPI.transfer(0x02);
  SPI.transfer(0x52);
  SPI.transfer(0b0000111);
  digitalWrite(SPI_SS, HIGH);
  POLLING();
  READING();
}

void HALT(){
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(SEND);
  SPI.transfer(0x04);
  SPI.transfer(0x02);
  SPI.transfer(0x50);
  SPI.transfer(0x00);
  digitalWrite(SPI_SS, HIGH);
  POLLING();
  READING();
}

void READTAG0(){
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(SEND);
  SPI.transfer(0x04);
  SPI.transfer(0x03);
  SPI.transfer(0x30);
  SPI.transfer(0x00);
  SPI.transfer(0x28);
  digitalWrite(SPI_SS, HIGH);
  POLLING();
  READING();
}

void ANTICOL1(){
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(SEND);
  SPI.transfer(0x04);
  SPI.transfer(0x03);
  SPI.transfer(0x93);
  SPI.transfer(0x20);
  SPI.transfer(0x08);
  digitalWrite(SPI_SS, HIGH);
  POLLING();
  READING();
  delay(5);
}

void SELECT1(){
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(SEND);
  SPI.transfer(0x04);
  SPI.transfer(0x08);
  SPI.transfer(0x93);
  SPI.transfer(0x70);
  SPI.transfer(0x88);
  SPI.transfer(0x04);
  SPI.transfer(0xEB);
  SPI.transfer(0xE6);
  SPI.transfer(0x81);
  SPI.transfer(0x28);
  digitalWrite(SPI_SS, HIGH);
  POLLING();
  READING();
}

void ANTICOL2(){
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(SEND);
  SPI.transfer(0x04);
  SPI.transfer(0x03);
  SPI.transfer(0x95);
  SPI.transfer(0x20);
  SPI.transfer(0x08);
  digitalWrite(SPI_SS, HIGH);
  POLLING();
  READING();
  delay(5);

}

void SELECT2(){
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(SEND);
  SPI.transfer(0x04);
  SPI.transfer(0x08);
  SPI.transfer(0x95);
  SPI.transfer(0x70);
  SPI.transfer(0x4A);
  SPI.transfer(0x5C);
  SPI.transfer(0x71);
  SPI.transfer(0x80);
  SPI.transfer(0xE7);
  SPI.transfer(0x28);
  digitalWrite(SPI_SS, HIGH);
  POLLING();
  READING();
}

void GETVERSION(){
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(SEND);
  SPI.transfer(0x04);
  SPI.transfer(0x02);
  SPI.transfer(0x60);
  SPI.transfer(0x28);
  digitalWrite(SPI_SS, HIGH);
  POLLING();
  READING(0x04);
}

void WRITETAG(byte Adress){
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(SEND);
  SPI.transfer(0x04);
  SPI.transfer(0x07);
  SPI.transfer(0xA2);
  SPI.transfer(Adress);
  SPI.transfer(0x42);
  SPI.transfer(0x72);
  SPI.transfer(0x75);
  SPI.transfer(0x68);
  SPI.transfer(0x28);

  digitalWrite(SPI_SS, HIGH);
  POLLING();
  READING();
}

void READTAG(byte Adress){
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(SEND);
  SPI.transfer(0x04);
  SPI.transfer(0x03);
  SPI.transfer(0x30);
  SPI.transfer(Adress);
  SPI.transfer(0x28);
  digitalWrite(SPI_SS, HIGH);
  POLLING();
  READING(0x01);
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

  SENDING(Echo,0x00);
  SENDING(IDN,0x00);
  SENDING(ProtocolSelect,0x02);
  //REQUESTA();

}

void loop() {
  
  delay(1500);
  Serial.print("Loop ");
  Serial.println(l);
  l=l+1;

  REQUESTA();

  

  if(resp!=0x80){
    REQUESTA();
  }
  else{
    Serial.print("Tag detected");
    Serial.println();
    ANTICOL1();
    SELECT1();
    ANTICOL2();
    SELECT2();
    GETVERSION();
    READTAG(0x04);
    HALT();/*
    REQUESTA();
    READTAG0();
    GETVERSION();
    READTAG(0x04);
    HALT();*/
/*
    WRITETAG(0x04);
    WRITETAG(0x05);
    WRITETAG(0x06);
    WRITETAG(0x07);*/
  }
  Serial.println(); 
  
}
