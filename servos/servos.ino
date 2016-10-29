#include <Servo.h> 
 
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

  uint16_t trottle_0;
  uint16_t turn_0;
  uint16_t front_0;
  uint16_t right_0;
  uint16_t trottle_cur; 

int val;
 
void setup() 
{ 
  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(11);

  delay(10);
  trottle_0 = analogRead(A0);
  turn_0 = analogRead(A1);
  front_0 = analogRead(A2);
  right_0 = analogRead(A3);   
} 


void loop() 
{ 
  uint16_t trottle = analogRead(A0);
  uint16_t turn = analogRead(A1);
  uint16_t front = analogRead(A2);
  uint16_t right = analogRead(A3);   

  trottle = map(trottle, 0, 1023, 0, 180);
  servo1.write(val);

  turn = map(turn, 0, 1023, 0, 180);
  servo2.write(turn);

  front = map(front, 0, 1023, 0, 180);
  servo3.write(front);

  delay(15);
} 

