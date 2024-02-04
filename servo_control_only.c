#include <Servo.h>

Servo myservo;  // Create a servo object

void setup()
{
  myservo.attach(9);  // Attaches the servo on pin 9
}

void loop()
{
  myservo.write(0);  // Move the servo to 0 degrees
  delay(1000);      // Wait for 1 second
  myservo.write(90); // Move the servo to 90 degrees
  delay(1000);      // Wait for 1 second
  myservo.write(180); // Move the servo to 180 degrees
  delay(1000);      // Wait for 1 second
}
