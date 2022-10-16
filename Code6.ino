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

void READING(byte control=0xFF){ //READ from CR95HF
  digitalWrite(SPI_SS, LOW);
  SPI.transfer(READ);
  byte resp = SPI.transfer(0x00);
  if(control==0x02 && resp==0x00){ // On souhaite confirmer que le protocole a été selectionné correctement
  Serial.print("Successful protocol selection");
  Serial.println();
  }
  byte len = SPI.transfer(0x00);
  for (int i = 0; i < len; i++) { //Lecture et affichage successif des données
    byte data = SPI.transfer(0x00);
    Serial.print(char(data));
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

}

void loop() {

}
