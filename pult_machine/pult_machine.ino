#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

byte data[16];

void setup(){
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"ADDR1");//ADDR1
  Mirf.setTADDR((byte *)"ADDR1");//
     
  Mirf.payload = 16;
  Mirf.channel = 1;
  Mirf.config();
  delay(10);
}


void loop(){
{
  if (Mirf.dataReady()) 
  {
    Mirf.getData(data);
    if (data[0] != 'M')
      return;
    uint16_t trottle = data[1] + (data[2] << 8);
    trottle = 1024 + trottle - trottle_0;
    if (trottle > 1030) 
    {
      trottle = (trottle - 1024) / 2;
      if (trottle > 255)
        trottle = 255;
      analogWrite(3, (uint8_t)trottle);
      analogWrite(5, 0);
    } 
    else if (trottle < 1018)
    {
      trottle = (1024 - trottle) / 2;
      if (trottle > 255)
        trottle = 255;
      analogWrite(5, (uint8_t)trottle);
      analogWrite(3, 0);
    }
    else
    {
      analogWrite(5, 0);
      analogWrite(3, 0);
   
    }
    uint16_t yaw = data[3] + (data[4] << 8);
    yaw = 1024 + yaw - yaw_0;
    if (yaw > 1030) 
    {
      yaw = (yaw - 1024) / 7;
      if (yaw > 255)
        yaw = 255;
      analogWrite(6, (uint8_t)yaw);
      analogWrite(9, 0);
    } 
    else if (yaw < 1018)
    {
      yaw = (1024 - yaw) / 7;
      if (yaw > 255)
        yaw = 255;
      analogWrite(9, (uint8_t)yaw);
      analogWrite(6, 0);
    }
    else
    {
      analogWrite(6, 0);
      analogWrite(9, 0);
   
    }
  }
  delay(50);
}


