const int myowarePin = A0; // Analog pin connected to MyoWare sensor
const int pwmPin = 9; // PWM pin connected to the motor driver for actuator control
// Adjust these thresholds based on testing and your application needs
const int muscleFlexThreshold = 300; // Threshold for muscle activity to trigger actuator movement
const int maxSensorValue = 1023; // Maximum analog read value for MyoWare sensor

void setup() {
  pinMode(pwmPin, OUTPUT);
  Serial.begin(9600); // Start serial communication for debugging
}

void loop() {
  int sensorValue = analogRead(myowarePin); // Read the MyoWare sensor value
  Serial.println(sensorValue); // Debugging: Print the sensor value
  
  if (sensorValue > muscleFlexThreshold) {
    // Map the sensor value to a PWM value (0-255)
    int pwmValue = map(sensorValue, muscleFlexThreshold, maxSensorValue, 0, 255);
    analogWrite(pwmPin, pwmValue); // Control the actuator based on muscle activity
  } else {
    analogWrite(pwmPin, 0); // Stop the actuator if below threshold
  }
  
  delay(10); // Short delay for stability
}
