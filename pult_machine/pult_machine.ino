#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#define DEADBAND 10
#define YAW_DELIM 3

byte data[16];
uint8_t okCounter = 0;

void setup(){
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"ADDR1");//ADDR1
  Mirf.setTADDR((byte *)"ADDR1");//
     
  Mirf.payload = 16;
  Mirf.channel = 1;
  Mirf.config();
  delay(10);
  Mirf.powerUpRx(); 
  //Serial.begin(28800);
  //Serial.print("Started...");
}

void loop() 
{
  if (Mirf.dataReady()) 
  {
    Mirf.getData(data);
    if (data[0] == 'M')
    {
      uint16_t trottle = data[2] + (data[1] << 8);
      if (trottle > 1024 + DEADBAND) 
      {
        trottle = (trottle - 1024) / 2;
        if (trottle > 255)
          trottle = 255;
        analogWrite(3, (uint8_t)trottle);
        analogWrite(5, 0);
      } 
      else if (trottle < 1024 - DEADBAND)
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
      //Serial.println(trottle);
      uint16_t yaw = data[4] + (data[3] << 8);
      if (yaw > 1024 + DEADBAND) 
      {
        yaw = (yaw - 1024) / YAW_DELIM;
        if (yaw > 255)
          yaw = 255;
        analogWrite(6, (uint8_t)yaw);
        analogWrite(9, 0);
      } 
      else if (yaw < 1024 - DEADBAND)
      {
        yaw = (1024 - yaw) / YAW_DELIM;
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
      okCounter = 10;
  //    Serial.print(yaw);
  //    Serial.print("Data...");
    }
  }
  if (okCounter == 1)//failsafe
  {
    analogWrite(3, 0);
    analogWrite(5, 0);   
    analogWrite(6, 0);
    analogWrite(9, 0);   
    okCounter = 0;
  } 
  else if (okCounter > 1)
  {
    okCounter--; 
  }
  delay(50);
}

