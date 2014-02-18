
// Don't use Serial.print()

#include <Wire.h>

unsigned long waitTimeRef;
enum pulseState_t
{
  pulse,
  wait
};
pulseState_t pulseState;

enum Pins
{
  Left = A0,
  Right = A1,
  Ball = A2,
  Pulse = 12,
  Led = 13,
};

uint16_t left = 0;
uint16_t right = 0;
uint16_t ball = 0;

void setup()
{
  digitalWrite(Led, true);
  Wire.begin(19);                // join i2c bus with address #38
  Wire.onRequest(requestEvent); // register event
  
  pinMode(Pulse, OUTPUT);
  pinMode(Led, OUTPUT);
  digitalWrite(Led, false);
  //Serial.begin(9600);
  pulseState = wait;
  
}
int Round(float value)
{
 return (int)(value + 0.5); 
}
void Sleep(int time)
{
  unsigned long startTime = millis();
  
  while(millis() - startTime < time)
  {
  }
}
void loop()
{
  
  switch(pulseState){
    case pulse:
        digitalWrite(Pulse, HIGH); 
        Sleep(1);
        digitalWrite(Pulse, LOW);
        waitTimeRef = millis();
        pulseState = wait;
    break;
     
    case wait:
      if(millis() - waitTimeRef < 100){
      }
      else{
        pulseState = pulse;
      }
    break;
  }
  left  = Round(0.501486896*(analogRead(Left))+2.151694603);
  //Serial.print(left);
  //Serial.print(", ");
  right = Round(0.501486896*(analogRead(Right))+2.151694603);
  //Serial.println(right);
  
  ball = analogRead(Ball);
  
  
  
  //static bool toggle = true;
  //toggle = !toggle;
  //digitalWrite(Led, toggle);
  delay(100);
}
  
void requestEvent()
{
  uint8_t buffer[6];
  uint8_t array1[2];
  uint8_t array2[2];
  uint8_t array3[2];
  
  
  // Dark Magic
  memcpy(array1, &left, 2);
  memcpy(array2, &right, 2);
  memcpy(array3, &ball, 2);
  
  // More Dark Magic
  for(int i = 0; i < 2; i++)
  {
    buffer[i]     = array1[1 - i];
    buffer[i + 2] = array2[1 - i];
    buffer[i + 4] = array3[1 - i];
  }
  
  Wire.write(buffer, 6);
  for(int i = 0; i < 6; i++)
  {
    Serial.print(buffer[i]);
     Serial.print(", "); 
  }
  
}


