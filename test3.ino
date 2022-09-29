#include <SPI.h>

// CR95HF: Write, poll, read, reset
#define   CMD             0x00
#define   POLL            0x03
#define   READ            0x02
#define   RESET           0x01

#define   ECHO            0x55

const int _SSpin = 10; // CS
const int _IRQpin = 6; // INT_out : 2, INT_in : 6, tried both, barely to no difference
//const int _SI0pin = A3;
//const int _SI1pin = A0;

void WakeupCR95HF()
{
 //Serial.println("Wake up!");
 digitalWrite(_IRQpin, LOW);
 delay(100);
 digitalWrite(_IRQpin, HIGH);
 delay(100);
}

bool EchoResponse()
{
    byte len = 0;

  // write to CR95HF
  digitalWrite(_SSpin, LOW);
  delay(100);
  SPI.transfer(0x00);

  // Serial.print("echo code: ");
  // Serial.println(ECHO);

  SPI.transfer(0x55);
  //SPI.transfer(0x00);
  digitalWrite(_SSpin, HIGH);
  //delay(1000);

  // poll to CR95HF, is it ready?
  byte tmp = 0;

  digitalWrite(_SSpin, LOW);
  while (!tmp)
  {
   delay(100);
    //Note: whatever I send, I get the same thing back. I send 3, then tmp is 3. I send 8, tmp is 8.
    SPI.transfer(0x03);
    tmp = SPI.transfer(0);

   // Serial.print("Polling: ");
    Serial.println(tmp, BIN);

    tmp = (tmp & 0x08) >> 3;

    delay(100); 
  }
  digitalWrite(_SSpin, HIGH);
  delay(20);

  // ready to write
  byte res = 0;

  digitalWrite(_SSpin, LOW);
  delay(100);
  SPI.transfer(0x02);
  res = SPI.transfer(0);

 // Serial.print("reading: ");
 // Serial.println(res);

  delay(100);
  digitalWrite(_SSpin, HIGH);
  delay(100);

  if (res == ECHO)
  {
    return true;
  }
 return false;
}

void setup() {
Serial.begin(9600);

// I believe this is not possible on Arduino or not nessaccery, tried it anyway, no change
// pinMode(_SI0pin, OUTPUT);
// pinMode(_SI1pin, OUTPUT);

 pinMode(_SSpin, OUTPUT);
 pinMode(_IRQpin, OUTPUT);

// digitalWrite(_SI0pin, HIGH);
// digitalWrite(_SI1pin, LOW);

  digitalWrite(_IRQpin, HIGH); // wakeup
  delay(20);
  digitalWrite(_SSpin, HIGH);  // slave select, low = ready to write/read
  delay(20);

  SPI.begin();
  // 2 MHz max clock speed, Most Significant Bit first, SPI_MODE unknown (probably 0, all were tried)
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));

  WakeupCR95HF();

  while(!EchoResponse())
  {
    WakeupCR95HF();
  }
 // Serial.println("Connection established");
}

void loop() {
}
