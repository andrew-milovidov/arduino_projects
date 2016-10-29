#include <SPI.h>
#include <SdFat.h> // Includes the "Sdfat.h" library to read an SD card
#define SD_CS_PIN 4
SdFat SD; // Defines it for use of the library.
File myFile; // Defines it for use of the library.
#define bufSize 256
byte buf[bufSize];
//byte buf2[bufSize];
int bufPos;
unsigned long fsize;

void setup() {
  Serial.begin(9600); // Initializes the Serial communication
  pinMode(SD_CS_PIN, OUTPUT);
  reverse();
}
void loop() {}

void reverse() {
   if (!SD.begin(SD_CS_PIN,SPI_FULL_SPEED)) {
    Serial.println("initialization failed!");
    return;
  } else {
    Serial.println("initialization done.");
  }
  if (SD.exists("out.wav")) {
    SD.remove("out.wav");
  }

  myFile = SD.open("soun.wav", O_READ);
  if (!myFile) {
    Serial.println("opening for read failed");
    return;
  } else {
    Serial.println("opening for read ok");
  }
  fsize = myFile.size();
  Serial.print("fsize= ");
  Serial.println(fsize);

  File outFile = SD.open("out.wav", FILE_WRITE);
  if (!outFile) {
    Serial.println("opening for WRITE failed");
    return;
  }
  // copying header
  myFile.seek(0);
  myFile.readBytes(buf, 43);
  outFile.write(buf, 43);

  //ВАРИАНТ прохода величиной буфера с последующей инверсией буфера
  // целые проходы величиной буфера с конца файла к началу
 /* for (unsigned long i = 1; i < (fsize - 43) / bufSize; i++) {
    myFile.seek(fsize - i * bufSize);
    myFile.readBytes(buf, bufSize);
    for (byte l = 0; l < bufSize; l++) { //перевертыш
      buf2[l] = buf[bufSize - l];
    }
    outFile.write(buf2, bufSize);
  }
  // остаток файла с 44 байта до конца целых проходов
  myFile.seek(44);
  myFile.readBytes(buf, (fsize - 43) % bufSize);
  for (byte l = 0; l < bufSize; l++) { //перевертыш
    buf2[l] = buf[bufSize - l];
  }
  outFile.write(buf2, bufSize);
*/

// ВАРИАНТ побайтового прохода с одним буфером
  for (unsigned long i = fsize; i >= 44; i--) {
    myFile.seek(i);
    bufPos = (fsize - i + bufSize) % bufSize;
    buf[bufPos] = myFile.peek();
    if ((bufPos == bufSize - 1) || i == 44) {
      if (i != 0) {
        outFile.write(buf, bufSize);
      } else {
        outFile.write(buf, bufPos + 1);
      }
    }
  }
  
  outFile.close();
  myFile.close();
  Serial.println("reversing done."); 
}

