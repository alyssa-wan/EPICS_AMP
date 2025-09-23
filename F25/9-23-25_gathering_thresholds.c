// V Just don't touch the stuff below just know that it works
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if (defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__) || defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_MINI) || defined(ARDUINO_AVR_ETHERNET) || defined(ARDUINO_AVR_FIO) || defined(ARDUINO_AVR_BT) || defined(ARDUINO_AVR_LILYPAD) || defined(ARDUINO_AVR_PRO) || defined(ARDUINO_AVR_NG) || defined(ARDUINO_AVR_UNO_WIFI_DEV_ED) || defined(ARDUINO_AVR_DUEMILANOVE) || defined(ARDUINO_AVR_FEATHER328P) || defined(ARDUINO_AVR_METRO) || defined(ARDUINO_AVR_PROTRINKET5) || defined(ARDUINO_AVR_PROTRINKET3) || defined(ARDUINO_AVR_PROTRINKET5FTDI) || defined(ARDUINO_AVR_PROTRINKET3FTDI))
  #define USE_TIMER_1 true
#else
  #define USE_TIMER_3 true
#endif
#define _PWM_LOGLEVEL_ 3
#if (_PWM_LOGLEVEL_ > 3)
  #if USE_TIMER_1
    #warning Using Timer1
  #elif USE_TIMER_1
    #warning Using Timer3
  #endif
#endif
#define USING_MICROS_RESOLUTION true  //false
#define HW_TIMER_INTERVAL_MS 0.1f
#define HW_TIMER_INTERVAL_FREQ 10000L
#define USING_PWM_FREQUENCY true
volatile uint32_t startMicros = 0;

#include "AVR_Slow_PWM.h"
#include <MsTimer2.h>

AVR_Slow_PWM ISR_PWM;
void TimerHandler() {
  ISR_PWM.run();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ^ Just don't touch the stuff above just know it works

/* These pins are used to pick which sensor to read from enbit should always 
 * be set to high to run. The other selection bits should be set like a binary
 * number to choose which finger to read from (ex: to read from pinky s1= low,
 * s2 = low, s3 = low & to read from ring s1 = high, s2 = low, s2 = low).
 */
const int enbit         = 2;
const int selectionbit0 = 4;
const int selectionbit1 = 7;
const int selectionbit2 = 8;

/* These pins are the pins used to send the signal to the actuators to trigger 
 * movement.
 */
const int pinky  =  3;
const int ring  =  5;
const int middle =  9;
const int index   =  10;// 10
const int thumb  =  11;

/* This is the pin that takes in the data from the sensors. */
int  sensor_output_pin = A0;

/* Set these to the actuators you are sending to.
 */
const int num_pins = 1;
const int num_fingers = 1;
uint32_t PWM_Pin[] = { pinky, ring, middle, index, thumb}; 

/* These are simply for the sake of the for loop descisions.
 */
const int pinkySelected  = 0;
const int ringSelected   = 1;
const int middleSelected = 2;
const int indexSelected  = 3;
const int thumbSelected  = 4;

/* These are to hold the values of the current thresholds for each finger.
 */
long sensor_value     = 0;
long triggerArr[]     = { 0, 0, 0, 0, 0 };
long sumArr[]         = { 0, 0, 0, 0, 0 };
long sumArrFlexed[]         = { 0, 0, 0, 0, 0 };
long sumArrRelaxed[]         = { 0, 0, 0, 0, 0 };
long prevTriggerArr[] = { 0, 0, 0, 0, 0 };

/* These are a whole bunch of values that affect when the actuators are triggered.
 */
const int  fullScale     = 1023;
const int  threshold     = fullScale / 10;
const long diffThreshold = fullScale / 10;
const long  windowSize    = 30;
const long divideSize = 1;
      int  windowIndex   = 0;
int temp_trigger = 75; //75
int temp_difftrigger = 30;
int triggerCooldown = 5;
bool extended = false;
bool retracted = true;

long just_triggered[] = {0, 0, 0, 0, 0};


/* These are a whole bunch of values that affect the actuator movement.
 */
const float PWM_PERIOD_MS      = 20.0;
const float RETRACTED_PULSE_MS = 2.0;
const float EXTENDED_PULSE_MS  = 1.0;
const float RETRACTED_DUTY     = (RETRACTED_PULSE_MS / PWM_PERIOD_MS) * 100.0;
const float EXTENDED_DUTY      = (EXTENDED_PULSE_MS / PWM_PERIOD_MS) * 100.0;
      float curr_duty_cycle    = 5.0f;
const float PWM_Freq1          = 50.0f;


/* These hold the open channels to the actuators they need to be deleted every time
 */
int channelNum[5] = { -1, -1, -1, -1, -1 };
int start = 0;
int relaxed = 0;
int flexed = -1;
int avgRelaxed[] = {0,0,0,0,0};
int avgFlexed[] = {0,0,0,0,0};


long initialThreshold[] = {0,0,0,0,0};
const int thresholdWindowSize = windowSize * 6;

void readSensor() {
  if (start != -1) {

    if (relaxed != -1) {
      if (relaxed == 1) {
       Serial.print("Relax your arm for 5 seconds\n");
      }
      initialThresholdGatheringRelaxed();
      relaxed++;
    
    if (relaxed == thresholdWindowSize ) {
      relaxed = -1;
      flexed = 0;
      Serial.print("Flex your arm for 5 seconds\n");
    }
    }
    if (flexed != -1) {

      initialThresholdGatheringFlexed();
      flexed++;
    }
    if (flexed > thresholdWindowSize ) {
      for (int i = 0; i < num_fingers; i++) {

      avgRelaxed[i] = sumArrRelaxed[i] / (divideSize * 6) ;
      avgFlexed[i] = sumArrFlexed[i] / (divideSize * 6);
      initialThreshold[i] = (avgRelaxed[i] + avgFlexed[i]) /2;
      
      Serial.print("threshold = ");
      Serial.print(sumArrRelaxed[i]);
      Serial.print("\n");
      Serial.print("threshold = ");
      Serial.print(avgRelaxed[i]);
      Serial.print("\n");
      Serial.print("threshold = ");
      Serial.print(sumArrFlexed[i]);
      Serial.print("\n");
      Serial.print("threshold = ");
      Serial.print(avgFlexed[i]);
      Serial.print("\n");
      Serial.print("threshold = ");
      Serial.print(initialThreshold[i]);
      Serial.print("\n");


      start = -1;
      }
    }
  } else{
  
  for (int i = 0; i < num_fingers; i++) { // 1 is usually num_pins
    switch (i) {
      case pinkySelected:
      //  digitalWrite(selectionbit0, LOW); digitalWrite(selectionbit1, LOW); digitalWrite(selectionbit2, LOW);
        break;
      case ringSelected:
        digitalWrite(selectionbit0, HIGH); digitalWrite(selectionbit1, LOW); digitalWrite(selectionbit2, LOW);
        break;
      case middleSelected:
        digitalWrite(selectionbit0, LOW); digitalWrite(selectionbit1, HIGH); digitalWrite(selectionbit2, LOW);
        break;
      case indexSelected:
        digitalWrite(selectionbit0, HIGH); digitalWrite(selectionbit1, HIGH); digitalWrite(selectionbit2, LOW);
        break;
      case thumbSelected:
        digitalWrite(selectionbit0, LOW); digitalWrite(selectionbit1, LOW); digitalWrite(selectionbit2, HIGH);
        break;
    }
    //delay(5);
    sensor_value = labs(analogRead(A0));
   // Serial.println(sensor_value);
    //if (just_triggered[i] == 0) {
    sumArr[i] += sensor_value;
    //}

    //Serial.println(sensor_value);

  }

  windowIndex++;

  if (windowIndex == (windowSize - 1)) {
    for (int i = 0; i < num_fingers; i++) { // 1 is usually num_pins
      //Serial.print("Sum = ");
      Serial.println(sumArr[i]);
      just_triggered[i] = 0;
      triggerArr[i] = sumArr[i] / divideSize;
      windowIndex = 0; sumArr[i] = 0; // reset for next round
      //Serial.print("Trigger = ");
      //Serial.println(triggerArr[i]);
      //if (abs(prevTriggerArr[i] - triggerArr[i]) > diffThreshold) {
      if ((abs(prevTriggerArr[i] - triggerArr[i]) > temp_difftrigger) && (just_triggered[i] <= 0)) {
      //if ((just_triggered <= 0)) {

          just_triggered[i] = triggerCooldown;
        //if (triggerArr[i] > threshold) {
        if ((triggerArr[i] > initialThreshold[i]) && !retracted) {
          //setFinger(i, HIGH);
          setFinger(i, HIGH);
          //setFinger(1, HIGH);
         // setFinger(2, HIGH);


          retracted = true;
          extended = false;
          just_triggered[i] = triggerCooldown;

        } else if ((triggerArr[i] <= initialThreshold[i]) && !extended) {
          //setFinger(i, LOW);
          setFinger(i, LOW);
          //setFinger(1, LOW);
          //setFinger(2, LOW);

          retracted = false;
          extended = true;
          just_triggered[i] = triggerCooldown;

        }
        prevTriggerArr[i] = triggerArr[i];

      }

      just_triggered[i]--;

    }
  }
  }

}

void setFinger(int i, int status) {
//  Serial.print("Set finger");
  if (channelNum[i] != -1) {
    ISR_PWM.deleteChannel((unsigned)channelNum[i]);
  }
  if (status == 1) {
    curr_duty_cycle = RETRACTED_DUTY;
    //Serial.print("Setting Retracted"); Serial.println(i);
  } else {
    curr_duty_cycle = EXTENDED_DUTY;
    //Serial.print("Setting Extended"); Serial.println(i);
  }
  channelNum[i] = ISR_PWM.setPWM(PWM_Pin[i], PWM_Freq1, curr_duty_cycle);
 delay(50);
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  delay(1000);  // Give time for Serial to settle

  MsTimer2::set(10, readSensor);  // 500ms period
  MsTimer2::start();
  
  pinMode(selectionbit0, OUTPUT);
  pinMode(selectionbit1, OUTPUT);
  pinMode(selectionbit2, OUTPUT);
  pinMode(enbit        , OUTPUT);

  digitalWrite(selectionbit0, LOW);
  digitalWrite(selectionbit1, LOW);
  digitalWrite(selectionbit2, LOW);
  digitalWrite(enbit        , HIGH);

  #if USING_HW_TIMER_INTERVAL_MS
    #if USE_TIMER_1  
      ITimer1.init();
      ITimer1.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler);
    #elif USE_TIMER_3
      ITimer3.init();
      ITimer3.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler);
  #endif
  #else
    #if USE_TIMER_1
      ITimer1.init();
      ITimer1.attachInterrupt(HW_TIMER_INTERVAL_FREQ, TimerHandler);
    #elif USE_TIMER_3
      ITimer3.init();
      ITimer3.attachInterrupt(HW_TIMER_INTERVAL_FREQ, TimerHandler);
    #endif
  #endif

}

void initialThresholdGatheringRelaxed()
{

  for (int i = 0; i < num_fingers; i++) { // 1 is usually num_pins
    switch (i) {
      case pinkySelected:
      //  digitalWrite(selectionbit0, LOW); digitalWrite(selectionbit1, LOW); digitalWrite(selectionbit2, LOW);
        break;
      case ringSelected:
        digitalWrite(selectionbit0, HIGH); digitalWrite(selectionbit1, LOW); digitalWrite(selectionbit2, LOW);
        break;
      case middleSelected:
        digitalWrite(selectionbit0, LOW); digitalWrite(selectionbit1, HIGH); digitalWrite(selectionbit2, LOW);
        break;
      case indexSelected:
        digitalWrite(selectionbit0, HIGH); digitalWrite(selectionbit1, HIGH); digitalWrite(selectionbit2, LOW);
        break;
      case thumbSelected:
        digitalWrite(selectionbit0, LOW); digitalWrite(selectionbit1, LOW); digitalWrite(selectionbit2, HIGH);
        break;
    }
    //delay(5);
    sensor_value = labs(analogRead(A0));
  // Serial.println(sensor_value);
    //if (just_triggered[i] == 0) {
    sumArrRelaxed[i] += sensor_value;
    //}

  //  Serial.println(sensor_value);

  }

}
void loop() {}


void initialThresholdGatheringFlexed() {



  for (int i = 0; i < num_fingers; i++) { // 1 is usually num_pins
    switch (i) {
      case pinkySelected:
      //  digitalWrite(selectionbit0, LOW); digitalWrite(selectionbit1, LOW); digitalWrite(selectionbit2, LOW);
        break;
      case ringSelected:
        digitalWrite(selectionbit0, HIGH); digitalWrite(selectionbit1, LOW); digitalWrite(selectionbit2, LOW);
        break;
      case middleSelected:
        digitalWrite(selectionbit0, LOW); digitalWrite(selectionbit1, HIGH); digitalWrite(selectionbit2, LOW);
        break;
      case indexSelected:
        digitalWrite(selectionbit0, HIGH); digitalWrite(selectionbit1, HIGH); digitalWrite(selectionbit2, LOW);
        break;
      case thumbSelected:
        digitalWrite(selectionbit0, LOW); digitalWrite(selectionbit1, LOW); digitalWrite(selectionbit2, HIGH);
        break;
    }
    //delay(5);
    sensor_value = labs(analogRead(A0));
  // Serial.println(sensor_value);
    //if (just_triggered[i] == 0) {
    sumArrFlexed[i] += sensor_value;
    //}

  //  Serial.println(sensor_value);

  }

  
}


