/*
  MsTimer2 is a small and very easy to use library to interface Timer2 with
  humans. It's called MsTimer2 because it "hardcodes" a resolution of 1
  millisecond on timer2
  For Details see: http://www.arduino.cc/playground/Main/MsTimer2
*/
#include <MsTimer2.h>

// Read sensors each half second

const int myowarePin = A0; // Analog pin connected to MyoWare sensor
const int windowSize = 50; // Size of the trigger window

int triggerWindow[windowSize];
int windowIndex = 0;
int trigger;

void readSensor()
{

  int sensorValue = analogRead(myowarePin); // Read the MyoWare sensor value

  sensorValue = abs(sensorValue);


  triggerWindow[windowIndex] = sensorValue;
  
  if (windowIndex == (windowSize - 1)) {

    long sum = 0;
    for (int i = 0; i < windowSize; i++) {
      sum += triggerWindow[i];
      triggerWindow[i] = 0;
    }

    trigger = sum / windowSize;
    Serial.println(trigger) ;
  }
  windowIndex = (windowIndex + 1) % windowSize;

  

 // Serial.println(sensorValue); // Debugging: Print the sensor value
}

void setup()
{
  Serial.begin(19200);
  MsTimer2::set(1, readSensor); // 500ms period
  MsTimer2::start();
}

void loop()
{
}