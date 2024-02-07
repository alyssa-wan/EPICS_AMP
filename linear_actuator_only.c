const int pwmPin = 9; // PWM pin connected to the driver
const int actuatorUpPWM = 220; // PWM value for moving up
const int actuatorDownPWM = 120; // PWM value for moving down
const long duration = 1000; // Duration for each movement in milliseconds

void setup()
{
  pinMode(pwmPin, OUTPUT);
}

void loop()
{
  // Move the actuator up
  analogWrite(pwmPin, actuatorUpPWM);
  delay(duration); // Wait for the actuator to move

  // Move the actuator down
  analogWrite(pwmPin, actuatorDownPWM);
  delay(duration); // Wait for the actuator to move
}
