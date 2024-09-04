#include <stdio.h>

const int threshValue = 10; // Required threshold to turn servo
const int actuatorUpPWM = 220; // PWM value for moving up
const int actuatorDownPWM = 120; // PWM value for moving down
const int sensorPin = A0; // Analog pin connected to the sensor
const int windowSize = 100; // Size of the trigger window
const int pwmPin1 = 9;
const int pwmPin2 = 10;
const int pwmPin3 = 11;

int triggerWindow[windowSize];
int windowIndex = 0;
int count = 0;
int randomNumber = 0;

void setup() {
  pinMode (pwmPin1, OUTPUT); // Attaching servo to pin
  pinMode (pwmPin2, OUTPUT); // Attaching servo to pin
  pinMode (pwmPin3, OUTPUT); // Attaching servo to pin
  Serial.begin(9600); // Start serial communication
  randomSeed(analogRead(0));
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


  if (trigger > threshValue)
  {
    analogWrite(pwmPin, actuatorUpPWM);
    count += 1;
  }
  else
  {
    analogWrite(pwmPin, actuatorDownPWM);
  } 

  if  (count == 3) 
  {
    int rand = random(1,4);
  }
  if (rand == 1)
  {
    analogWrite(pwmPin1, actuatorUpPWM);
    analogWrite(pwmPin2, actuatorUpPWM);
    analogWrite(pwmPin3, actuatorUpPWM);
    delay(500);
    rand = 0;
    count = 0;
  }
  if (rand == 2)
  {
    analogWrite(pwmPin1, actuatorUpPWM);
    analogWrite(pwmPin2, actuatorDownPWM);
    analogWrite(pwmPin3, actuatorUpPWM);
    delay(500);
    rand = 0;
    count = 0;
  }
  if (rand == 3)
  {
    analogWrite(pwmPin1, actuatorDownPWM);
    analogWrite(pwmPin2, actuatorDownPWM);
    analogWrite(pwmPin3, actuatorDownPWM);
    delay(500);
    rand = 0;
    count = 0;
  }

  
  //delay(10); // Delay in between reads for stability
}
