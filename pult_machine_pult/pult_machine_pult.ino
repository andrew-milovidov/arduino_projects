#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

  byte data[16];
  uint16_t trottle_0;
  uint16_t turn_0;
  uint16_t front_0;
  uint16_t right_0;
  uint16_t trottle_cur;


void setup(){
  pinMode(3, OUTPUT); 

  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"ADDR1");//ADDR1
  Mirf.setTADDR((byte *)"ADDR1");//
     
  Mirf.payload = 16;
  Mirf.channel = 1;
  Mirf.config();
  delay(10);
  trottle_0 = analogRead(A0);
  turn_0 = analogRead(A1);
  front_0 = analogRead(A2);
  right_0 = analogRead(A3);  
}


void loop() {
  
  uint16_t trottle = analogRead(A0);
  uint16_t turn = analogRead(A1);
  uint16_t front = analogRead(A2);
  uint16_t right = analogRead(A3);  
  
  trottle = 1024 - trottle_0 + trottle;
  right = 1024 - right_0 + right;
  front = 1024 - front_0 + front;
  turn = 1024 - turn_0 + turn;
  
  data[0] = 'M';
  data[1] = (trottle_cur >> 8) & 0xFF;
  data[2] = (trottle_cur) & 0xFF;
  data[3] = (turn >> 8) & 0xFF;
  data[4] = (turn) & 0xFF;
  data[5] = (right >> 8) & 0xFF;
  data[6] = (right) & 0xFF;
  data[7] = (front >> 8) & 0xFF;
  data[8] = (front) & 0xFF;
  mirfSend();
  delay(100);
}
  
  void mirfSend()
  {
    Mirf.send(data); 
    while(Mirf.isSending())
      ;
  }
