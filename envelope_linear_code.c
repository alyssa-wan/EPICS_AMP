const int pwmPin = 9; // PWM pin connected to the driver
const int actuatorUpPWM = 220; // PWM value for moving up
const int actuatorDownPWM = 120; // PWM value for moving down
const long duration = 500; // Duration for each movement in milliseconds
const int myowarePin = A0;
const int threshValue = 80;
const int windowSize = 500; // Size of the trigger window
int triggerWindow[windowSize];
int windowIndex = 0;

void setup() {
  pinMode(pwmPin, OUTPUT);
  Serial.begin(9600); // Start serial communication
}

void loop() {
  int sensorValue = analogRead(myowarePin); // Read the input on analog pin A0
  
  if (sensorValue < 0) {
    sensorValue = -sensorValue;
  }
  
  triggerWindow[windowIndex] = sensorValue;
  windowIndex = (windowIndex + 1) % windowSize;
  
  int sum = 0;
  for (int i = 0; i < windowSize; i++) {
    sum += triggerWindow[i];
  }
  
  int trigger = sum / windowSize;
  
  if (trigger > threshValue)
  {
    analogWrite(pwmPin, actuatorUpPWM);
  }
  else
  {
    analogWrite(pwmPin, actuatorDownPWM);
  }
  
  delay(300); // Delay in between reads for stability
}
