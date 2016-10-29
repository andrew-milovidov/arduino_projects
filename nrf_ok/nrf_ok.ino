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

int k = 0;

void setup(){
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
   
  Mirf.setRADDR((byte *)"serv1");
  
  /*
   * Set the payload length to sizeof(unsigned long) the
   * return type of millis().
   *
   * NB: payload on client and server must be the same.
   */
   
  Mirf.payload = 8;
  
  /*
   * Write channel and payload config then power up reciver.
   */
   
  Mirf.config();
    Mirf.setTADDR((byte *)"serv1");
}

void loop(){
  /*
   * A buffer to store the data.
   */

  Serial.println("Send");    
  byte data[Mirf.payload];
  data[0] = 't';
  data[1] = 'e';
  data[2] = 's';
  data[3] = 't';
  data[4] = (byte)k;
  k++;
  Mirf.send(data);
  Serial.println("Sent");    
  delay(50);
}
