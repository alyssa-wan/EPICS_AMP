#include <stdio.h>

const int threshValue = 7.5; // Required threshold to turn servo
const int actuatorUpPWM = 220; // PWM value for moving up
const int actuatorDownPWM = 120; // PWM value for moving down
const int sensorPin = A0; // Analog pin connected to the sensor
const int windowSize = 100; // Size of the trigger window
const int pwmPin = 9;

int triggerWindow[windowSize];
int windowIndex = 0;

void setup() {
  pinMode (pwmPin, OUTPUT); // Attaching servo to pin
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


  if (trigger > threshValue)
  {
    analogWrite(pwmPin, actuatorUpPWM);
  }
  else
  {
    analogWrite(pwmPin, actuatorDownPWM);
  }  
  
  //delay(10); // Delay in between reads for stability
}
