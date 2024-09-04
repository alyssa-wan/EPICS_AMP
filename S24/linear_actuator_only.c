const int pwmPin = 9; // PWM pin connected to the driver
const int actuatorUpPWM = 220; // PWM value for moving up
const int actuatorDownPWM = 120; // PWM value for moving down
const long duration = 500; // Duration for each movement in milliseconds
const int myowarePin = A0;
const int muscleFlexThreshold = 80;

void setup() {
  pinMode(pwmPin, OUTPUT);
  Serial.begin(9600); // Start serial communication for debugging
}

void loop()
{
  int sensorValue = analogRead(myowarePin); // Read the MyoWare sensor value
  Serial.println(sensorValue); // Debugging: Print the sensor value
  
  if (sensorValue > muscleFlexThreshold)
  {
    analogWrite(pwmPin, actuatorUpPWM);
  }
  else
  {
    analogWrite(pwmPin, actuatorDownPWM);
  }  
  
  delay(duration); // Short delay for stability
}
