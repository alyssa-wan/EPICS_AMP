#include <MsTimer2.h>


#if (defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__) || defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_MINI) || defined(ARDUINO_AVR_ETHERNET) || defined(ARDUINO_AVR_FIO) || defined(ARDUINO_AVR_BT) || defined(ARDUINO_AVR_LILYPAD) || defined(ARDUINO_AVR_PRO) || defined(ARDUINO_AVR_NG) || defined(ARDUINO_AVR_UNO_WIFI_DEV_ED) || defined(ARDUINO_AVR_DUEMILANOVE) || defined(ARDUINO_AVR_FEATHER328P) || defined(ARDUINO_AVR_METRO) || defined(ARDUINO_AVR_PROTRINKET5) || defined(ARDUINO_AVR_PROTRINKET3) || defined(ARDUINO_AVR_PROTRINKET5FTDI) || defined(ARDUINO_AVR_PROTRINKET3FTDI))

// TODO: this is timer used so we can take out the if statement here
#define USE_TIMER_1 true
#else
#define USE_TIMER_3 true
#endif

// These define's must be placed at the beginning before #include "AVR_Slow_PWM.h"
// _PWM_LOGLEVEL_ from 0 to 4
// Don't define _PWM_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define _PWM_LOGLEVEL_ 3

#if (_PWM_LOGLEVEL_ > 3)
#if USE_TIMER_1
// TODO: we are only using timer 1 according to the if statement that chooses which timer
#warning Using Timer1

#elif USE_TIMER_1
#warning Using Timer3
#endif
#endif

#define USING_MICROS_RESOLUTION true  //false

// Default is true, uncomment to false
//#define CHANGING_PWM_END_OF_CYCLE     false

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "AVR_Slow_PWM.h"



// Don't change these numbers to make higher Timer freq. System can hang
#define HW_TIMER_INTERVAL_MS 0.1f
#define HW_TIMER_INTERVAL_FREQ 10000L

volatile uint32_t startMicros = 0;

// Init AVR_Slow_PWM, each can service 16 different ISR-based PWM channels
AVR_Slow_PWM ISR_PWM;

//////////////////////////////////////////////////////

void TimerHandler() {
  ISR_PWM.run();
}

//////////////////////////////////////////////////////

#define USING_PWM_FREQUENCY true

//////////////////////////////////////////////////////


// You can assign any interval for any timer here, in Hz
float PWM_Freq1 = 50.0f;
// You can assign any interval for any timer here, in Hz
float PWM_Freq2 = 50.0f;

// You can assign any interval for any timer here, in microseconds
uint32_t PWM_Period1 = 1000000 / PWM_Freq1;
// You can assign any interval for any timer here, in microseconds
uint32_t PWM_Period2 = 1000000 / PWM_Freq2;

// You can assign any duty_cycle for any PWM here, from 0-100
float PWM_DutyCycle1 = 5.0f;
// You can assign any duty_cycle for any PWM here, from 0-100
float PWM_DutyCycle2 = 7.5f;

// Channel number used to identify associated channel
int channelNum[5] = { -1, -1, -1, -1, -1 };

// You can assign pins here. Be carefull to select good pin to use or crash
#define THUMB 3
#define INDEX 5
// #define MIDDLE 9
//#define RING       10
//#define PINKY      11

#define  selectionbit0  4
#define  selectionbit1  7
#define  selectionbit2  8

#define  sensoroutputpin  A0

uint32_t PWM_Pin[] = { THUMB, INDEX };  // add PINKY if pin 11 works
#define NUM_OF_PINS 2


const int num_sensors = 2;
const int myowarePinArr[num_sensors] = {A0, A1};  // A2, A3, A4

//const int myowarePin = A1;  // Analog pin connected to MyoWare sensor

const int pinkySelected = 0;
const int ringSelected = 1;
const int middleSelected = 2;
const int indexSelected = 3;
const int thumbSelected = 4;
////////////////////////////////////////////////


long triggerArr[num_sensors] = { 0, 0 };
long sumArr[num_sensors] = { 0, 0 };
long prevTriggerArr[num_sensors] = { 0, 0 };

////////////////////////////////////////////////

const int fullScale = 1023;
const int threshold = fullScale / 4;
long trigger = 0;
long sum = 0;
const int windowSize = 80;
int windowIndex = 0;
long prevTrigger = 0;
const long diffThreshold = fullScale / 5;

void readSensor() {

  int sensorValueArr[num_sensors] = {0, 0};
  for (int i = 0; i < num_sensors; i++) {

  if (i == pinkySelected) {
    digitalWrite(selectionbit0, LOW);
    digitalWrite(selectionbit1, LOW);
    digitalWrite(selectionbit2, LOW);
  }
  if (i == ringSelected) {
    digitalWrite(selectionbit0, HIGH);
    digitalWrite(selectionbit1, LOW);
    digitalWrite(selectionbit2, LOW);
  }
  if (i == middleSelected) {
    digitalWrite(selectionbit0, LOW);
    digitalWrite(selectionbit1, HIGH);
    digitalWrite(selectionbit2, LOW);
  }
  if (i == indexSelected) {
    digitalWrite(selectionbit0, HIGH);
    digitalWrite(selectionbit1, HIGH);
    digitalWrite(selectionbit2, LOW);
  }
  if (i == thumbSelected) {
    digitalWrite(selectionbit0, LOW);
    digitalWrite(selectionbit1, LOW);
    digitalWrite(selectionbit2, HIGH);
  }
  // delay(50000);
  

  //int sensorValue = analogRead(myowarePin);  // Read the MyoWare sensor value

    sensorValueArr[i] = abs(analogRead(A0));  // Read the MyoWare sensor value

    sumArr[i] += sensorValueArr[i];
    //Serial.print("sensor val ");Serial.print(i); Serial.print(" = "); Serial.println(sumArr[i]);
    //Serial.print("sensor val ");Serial.print(i); Serial.print(" = "); Serial.println(sensorValueArr[i]);
  }

  //Serial.println(sensorValue); // Debugging: Print the sensor value

  // sensorValue = abs(sensorValue);

  //sum += sensorValue;
  windowIndex++;

  if (windowIndex == (windowSize - 1)) {
    for (int i = 0; i < num_sensors; i++) {
      triggerArr[i] = sumArr[i] / windowSize;
      //Serial.println(trigger);
      windowIndex = 0;
      sumArr[i] = 0;
      //Serial.print("trigger val ");Serial.print(i); Serial.print(" = "); Serial.println(triggerArr[i]);
      if (abs(prevTriggerArr[i] - triggerArr[i]) > diffThreshold) {
        if (triggerArr[i] > threshold) {

          Serial.println(triggerArr[i]);      // Debugging: Print the sensor value
          Serial.println(prevTriggerArr[i]);  // Debugging: Print the sensor value

          setFinger(i, HIGH);

        } else {
          setFinger(i, LOW);
        }
        prevTriggerArr[i] = triggerArr[i];
      }
    }
  }
}
const float PWM_PERIOD_MS = 20.0;
const float RETRACTED_PULSE_MS = 2.0;
const float EXTENDED_PULSE_MS = 1.0;
const float RETRACTED_DUTY = (RETRACTED_PULSE_MS / PWM_PERIOD_MS) * 100.0;
const float EXTENDED_DUTY = (EXTENDED_PULSE_MS / PWM_PERIOD_MS) * 100.0;
float curr_duty_cycle = 5.0f;


void setFinger(int i, int status) {

  if (channelNum[i] != -1) {
    ISR_PWM.deleteChannel((unsigned)channelNum[i]);
  }
  if (status == 1) {
    curr_duty_cycle = RETRACTED_DUTY;
  } else {
    curr_duty_cycle = EXTENDED_DUTY;
  }
  channelNum[i] = ISR_PWM.setPWM(PWM_Pin[i], PWM_Freq1, curr_duty_cycle);

  // delay(50);
  //Serial.println(curr_duty_cycle);
}


void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  delay(2000);

  MsTimer2::set(10, readSensor);  // 500ms period
  MsTimer2::start();

  pinMode(selectionbit0, OUTPUT);
  pinMode(selectionbit1, OUTPUT);
  pinMode(selectionbit2, OUTPUT);

  Serial.print(F("\nStarting ISR_Changing_PWM on "));
  Serial.println(BOARD_NAME);
  Serial.println(AVR_SLOW_PWM_VERSION);
  Serial.print(F("CPU Frequency = "));
  Serial.print(F_CPU / 1000000);
  Serial.println(F(" MHz"));

  // Timer0 is used for micros(), millis(), delay(), etc and can't be used
  // Select Timer 1-2 for UNO, 1-5 for MEGA, 1,3,4 for 16u4/32u4
  // Timer 2 is 8-bit timer, only for higher frequency
  // Timer 4 of 16u4 and 32u4 is 8/10-bit timer, only for higher frequency

#if USING_HW_TIMER_INTERVAL_MS

#if USE_TIMER_1  // TODO: we are using timer 1 according to the board picked if statement at the beginning?

  ITimer1.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,

  if (ITimer1.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler)) {
    Serial.print(F("Starting  ITimer1 OK, micros() = "));
    Serial.println(micros());
  } else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

#elif USE_TIMER_3

  ITimer3.init();

  if (ITimer3.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler)) {
    Serial.print(F("Starting  ITimer3 OK, micros() = "));
    Serial.println(micros());
  } else
    Serial.println(F("Can't set ITimer3. Select another freq. or timer"));

#endif
  =
#else

#if USE_TIMER_1  // TODO: we are using timer 1 according to the board picked if statement at the beginning?

  ITimer1.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,

  if (ITimer1.attachInterrupt(HW_TIMER_INTERVAL_FREQ, TimerHandler)) {
    Serial.print(F("Starting  ITimer1 OK, micros() = "));
    Serial.println(micros());
  } else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

#elif USE_TIMER_3

  ITimer3.init();

  if (ITimer3.attachInterrupt(HW_TIMER_INTERVAL_FREQ, TimerHandler)) {
    Serial.print(F("Starting  ITimer3 OK, micros() = "));
    Serial.println(micros());
  } else
    Serial.println(F("Can't set ITimer3. Select another freq. or timer"));

#endif

#endif
}
// TODO: Which of these do we want to keep
//#define ACTU_PWM_FREQ          63000.0f    // in hertz
//#define ACTU_PWM_PERIOD        1 / ACTU_PWM_FREQ

//#define RETRACTED_PULSE        2.0f
//#define EXTENDED_PULSE         1.0f

//#define RETRACTED_DUTY         RETRACTED_PULSE / ACTU_PWM_PERIOD   // max_duty
//#define EXTENDED_DUTY          EXTENDED_PULSE / ACTU_PWM_PERIOD    // min_duty

//#define MIN_STEP_SIZE          (1.0 / 50.0)f
//#define NUM_STEPS              50

int curr_step = 0;
float temp_int_float = 0.0f;

#define NUM_STEPS 5

void loop() {
  // TODO: weird things about this math: curr_step is an int so it might mess with stuff, we should put in our defines for this statement so we know what each thing is
  //curr_duty_cycle = ((curr_step - 0.0) * (0.1 - 0.05) / (5 - 0) + 0.05)*100;
  //Serial.print(F("Using PWM Freq = ")); Serial.print(PWM_Freq1); Serial.print(F(", PWM DutyCycle = ")); Serial.println(PWM_DutyCycle1);

  // for (int i = 0; i < NUM_OF_PINS; i++) {
  // You can use this with PWM_Freq in Hz
  //if over thresh()
  // channelNum[i] = ISR_PWM.setPWM(PWM_Pin[i], PWM_Freq1, curr_duty_cycle);

  //delay(50);
  //Serial.println(curr_duty_cycle);

  // ISR_PWM.deleteChannel((unsigned) channelNum[i]);
  //}

  // TODO: this is a wee bit messy the below statement would do the samething, we had it like this when we had curr_step being a float and now its an int so the mod should be fine
  // this one is the one I like:
  //  curr_step = (1 + curr_step) % NUM_STEPS;

  //temp_int_float = (int)(1 + temp_int_float) % NUM_STEPS;
  //curr_step = temp_int_float;
}
