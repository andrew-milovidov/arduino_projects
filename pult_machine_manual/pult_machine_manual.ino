
  uint16_t trottle_0;
  uint16_t yaw_0;


void setup(){
  pinMode(3, OUTPUT); 
  pinMode(5, OUTPUT); 
  pinMode(6, OUTPUT); 
  pinMode(9, OUTPUT); 
  trottle_0 = analogRead(A0);
  yaw_0 = analogRead(A1);
}


void loop(){
  
  uint16_t trottle = analogRead(A0);
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
  uint16_t yaw = analogRead(A1);
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
