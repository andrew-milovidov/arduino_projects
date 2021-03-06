    #include<Wire.h>
    const int MPU=0x68;  // I2C address of the MPU-6050
//    int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
    byte i, j, k = 0;
    int16_t values[7];
    int32_t akk[56];
    void setup(){
      Wire.begin();
      Wire.beginTransmission(MPU);
      Wire.write(0x6B);  // PWR_MGMT_1 register
      Wire.write(0);     // set to zero (wakes up the MPU-6050)
      Wire.endTransmission(true);
      Serial.begin(115200);
    }
    void loop(){
      Wire.beginTransmission(MPU);
      Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(MPU,14,true);  // request a total of 14 registers
      for (i = 0; i < 7; i ++)
      {
        akk[k * 8 + i] = (int16_t)(((int16_t)Wire.read()<<8) | Wire.read());
      }
      if (k < 7)
        k++;
      else
        k = 0;
      
//      AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
 //     AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
//      AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
//      Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
//      GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
//      GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
//      GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

      //Serial.print("AcX = "); 
      if (k==0)
      {
        for (i = 0; i < 7; i ++)
        {
          values[i] = 0;
          for (j = 0; j < 8; j ++)
          {
             values[i] += akk[7 * j + i]; 
          }
          values[i] /= 8;
        }  
        for (i = 0; i < 7; i ++)
        {
        Serial.print(values[i]);
        Serial.print(" | "); 
        }
              Serial.println();
      }

      delay(40);
    }

