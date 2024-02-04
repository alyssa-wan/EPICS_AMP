#include <Servo.h>  //servo library

Servo myservo1;  //initializes the servo
const int threshValue = 80; //required threshold to turn servo

void setup()
{
  myservo1.attach(3);  //pin attatchment of the servo
  Serial.begin(115200);  //serial plotter gaud
}

void loop()
{
  int sensorValue = analogRead(A3);  // read the input on analog pin A3
  Serial.println(sensorValue);  // print out the value you read

  if(sensorValue > threshValue)        
  { 
    myservo1.write(180);   //turns servo to flexed position  
  }
  else 
  { 
    myservo1.write(0);   //turns servo back to rest position
  } 

  delay(300);        // delay in between reads for stability
}
