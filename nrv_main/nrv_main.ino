/**
 * An Mirf example which copies back the data it recives.
 *
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 8
 * CSN -> 7
 *
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

void setup(){
  pinMode(3, OUTPUT); 
  Serial.begin(9600);
  
  /*
   * Set the SPI Driver.
   */

  Mirf.spi = &MirfHardwareSpi;
  
  /*
   * Setup pins / SPI.
   */
   
  Mirf.init();
  
  /*
   * Configure reciving address.
   */
   
  Mirf.setRADDR((byte *)"ADDR1");//ADDR1
  Mirf.setTADDR((byte *)"ADDR1");//
  
  /*
   * Set the payload length to sizeof(unsigned long) the
   * return type of millis().
   *
   * NB: payload on client and server must be the same.
   */
   
  Mirf.payload = 16;
  Mirf.channel = 1;
  Mirf.setCRC(1);  
  /*
   * Write channel and payload config then power up reciver.
   */
   
  Mirf.config();
  
  //Serial.println("Listening..."); 
}

  byte data[16];
  byte serialBuffer[20];

void loop(){
  /*
   * A buffer to store the data.
   */

  if (Mirf.dataReady()) 
  {
    Mirf.getData(data);
    serialBuffer[0] = 1;
    for(int i = 0; i < Mirf.payload; i++) {
      serialBuffer[i + 1] = data[i];
    }
    Serial.write(serialBuffer, 20);
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
      else if (serialBuffer[0] == 5)
      {
        pulse(2400);
        for(int i = 0; i < 16; i++)
        {
          pulse(serialBuffer[i + 1]);
        }
        
//        tone(3, 56000, serialBuffer[1]);
      }
    }
  }
}

void pulse(byte k)
{
  tone(3, 56000);
  //delayMicroseconds(k * 10);
  for(long i = 0; i < 40000; i++)
  {
      for(long j = 0; j < k; j++)
        __asm__("nop\n\t"); 
  }
  noTone(3);
  delayMicroseconds(600);
}

  void copyAndSend()
  {
    for(int i = 0; i < Mirf.payload; i++) 
    {
      data[i] = serialBuffer[i + 1];
    }
    Mirf.send(data); 
    while(Mirf.isSending())
      ;
  }
