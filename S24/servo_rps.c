#include <Servo.h>
#include <stdio.h>

Servo myservo1;
Servo myservo2;
const int threshValue = 10; // Required threshold to turn servo
const int servoPin1 = 9; // Pin attachment of the servo
const int servoPin2 = 10;
const int sensorPin = A0; // Analog pin connected to the sensor
const int windowSize = 100; // Size of the trigger window
int triggerWindow[windowSize];
int windowIndex = 0;
int count = 0;
int randomNumber = 0;

void setup() {
  myservo1.attach(servoPin1); // Attaching servo to pin
  myservo2.attach(servoPin2);
  Serial.begin(9600); // Start serial communication
}

void loop() {
  int sensorValue = analogRead(sensorPin); // Read the input on analog pin A3
  
  if (sensorValue < 0) {
    sensorValue = abs(sensorValue);
  }

  
  triggerWindow[windowIndex] = sensorValue;
  windowIndex = (windowIndex + 1) % windowSize;
  
  int sum = 0;
  for (int i = 0; i < windowSize; i++) {
    sum += triggerWindow[i];
  }
  
  int trigger = sum / windowSize;
  Serial.println(trigger) ;


  if (trigger > threshValue) {
    count += 1;
  }

  if (count >= 3)
  {
    randomNumber = random(1,4);
  }
  if (randomNumber == 1)
  {
    myservo1.write(0);
    myservo2.write(0);
    count = 0;
    randomNumber = 0;
    delay(800);
  }
  if (randomNumber == 2)
  {
    myservo1.write(180);
    myservo2.write(180);
    count = 0;
    randomNumber = 0;
    delay(800);
  }
  if (randomNumber == 3)
  {
    myservo1.write(180);
    myservo2.write(0);
    randomNumber = 0;
    delay(800);
  }

  
  delay(100);
}
