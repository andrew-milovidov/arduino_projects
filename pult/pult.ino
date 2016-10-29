#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

  byte data[16];
  byte serialBuffer[20];
  uint16_t trottle_0;
  uint16_t turn_0;
  uint16_t front_0;
  uint16_t right_0;
  uint16_t trottle_cur;


void setup(){
  pinMode(3, OUTPUT); 
  
  Serial.begin(28800);
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


void loop(){
  
  uint16_t trottle = analogRead(A0);
  uint16_t turn = analogRead(A1);
  uint16_t front = analogRead(A2);
  uint16_t right = analogRead(A3);  
  
  trottle = 1000 - trottle_0 + trottle;
  if (trottle > 1010)
  {
    trottle = (trottle - 1000) / 40;
    trottle_cur += trottle;
    if (trottle_cur > 1000)
      trottle_cur = 1000;
  }
  else if (trottle < 990)
  {
    trottle = 1000 - trottle;
    if (trottle_cur > trottle)
      trottle_cur -= trottle / 10;
    else
      trottle_cur = 0;
  }
  int k = 3;
  int k1 = k * 1000;
  k *= 2;
  turn = (k1 + turn - turn_0) / k;
  k = 8;
  k1 = k * 1000;
  k *= 2;
  front = (k1 + front - front_0) / k;
  right = (k1 + right - right_0) / k;
  data[0] = 1;
  data[1] = (trottle_cur >> 8) & 0xFF;
  data[2] = (trottle_cur) & 0xFF;
  data[3] = (turn >> 8) & 0xFF;
  data[4] = (turn) & 0xFF;
  data[5] = (right >> 8) & 0xFF;
  data[6] = (right) & 0xFF;
  data[7] = (front >> 8) & 0xFF;
  data[8] = (front) & 0xFF;
  if (Mirf.dataReady()) 
  {
    Mirf.getData(data);
    serialBuffer[0] = 2;
    for(int i = 0; i < Mirf.payload; i++) {
      serialBuffer[i + 1] = data[i];
    }
    Serial.write(serialBuffer, 20);
  }
  else
  {
   mirfSend();
   Serial.write(data, Mirf.payload);
  }
  if (Serial.available() > 0) 
  {
    byte b = Serial.readBytes((char *)&serialBuffer[0], 20);
    if (b > 0)
    {
      if (serialBuffer[0] == 2)
      {
        copyAndSend();
      }
      else if (serialBuffer[0] == 3)
      {
        Mirf.setChannel(serialBuffer[1]);
      }
      else if (serialBuffer[0] == (byte)'4')
      {
        Serial.write(serialBuffer, b);
      }
      else if (serialBuffer[0] == (byte)'5')
      {
        copyAndSend();
      }
      else if (serialBuffer[0] == (byte)'6')
      {
        Mirf.powerUpRx();
        Mirf.flushRx();
      }
      else if (serialBuffer[0] == 4)
      {
        Mirf.setRADDR((byte *)&serialBuffer[1]);//ADDR1
        Mirf.setTADDR((byte *)&serialBuffer[1]);//
      }
    }

  }
      delay(100);
}



  void copyAndSend()
  {
    for(int i = 0; i < Mirf.payload; i++) 
    {
      data[i] = serialBuffer[i + 1];
    }
    mirfSend();
  }
  
  void mirfSend()
  {
    Mirf.send(data); 
    while(Mirf.isSending())
      ;
  }
