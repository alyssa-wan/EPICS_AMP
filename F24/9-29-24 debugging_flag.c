#include "Mapf.h"
#include <MsTimer2.h>

// TODO: create debugging flag to activate and deactivate serial
#define SERIAL_DEBUGGING        true // false to turn off serial debugging flags

#if ( defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)  || \
        defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_MINI) ||    defined(ARDUINO_AVR_ETHERNET) || \
        defined(ARDUINO_AVR_FIO) || defined(ARDUINO_AVR_BT)   || defined(ARDUINO_AVR_LILYPAD) || defined(ARDUINO_AVR_PRO)      || \
        defined(ARDUINO_AVR_NG) || defined(ARDUINO_AVR_UNO_WIFI_DEV_ED) || defined(ARDUINO_AVR_DUEMILANOVE) || defined(ARDUINO_AVR_FEATHER328P) || \
        defined(ARDUINO_AVR_METRO) || defined(ARDUINO_AVR_PROTRINKET5) || defined(ARDUINO_AVR_PROTRINKET3) || defined(ARDUINO_AVR_PROTRINKET5FTDI) || \
        defined(ARDUINO_AVR_PROTRINKET3FTDI) )

  // TODO: this is timer used so we can take out the if statement here
  #define USE_TIMER_1     true
#else          
  #define USE_TIMER_3     true
#endif

// These define's must be placed at the beginning before #include "AVR_Slow_PWM.h"
// _PWM_LOGLEVEL_ from 0 to 4
// Don't define _PWM_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define _PWM_LOGLEVEL_      3

#if (_PWM_LOGLEVEL_ > 3)
  #if USE_TIMER_1
  // TODO: we are only using timer 1 according to the if statement that chooses which timer
    #warning Using Timer1
    
  #elif USE_TIMER_1
    #warning Using Timer3
  #endif
#endif

#define USING_MICROS_RESOLUTION       true    //false 

// Default is true, uncomment to false
//#define CHANGING_PWM_END_OF_CYCLE     false

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "AVR_Slow_PWM.h"



// Don't change these numbers to make higher Timer freq. System can hang
#define HW_TIMER_INTERVAL_MS        0.1f
#define HW_TIMER_INTERVAL_FREQ      10000L

volatile uint32_t startMicros = 0;

// Init AVR_Slow_PWM, each can service 16 different ISR-based PWM channels
AVR_Slow_PWM ISR_PWM;

//////////////////////////////////////////////////////

void TimerHandler()
{ 
  ISR_PWM.run();
}

//////////////////////////////////////////////////////

#define USING_PWM_FREQUENCY     true

//////////////////////////////////////////////////////

// You can assign pins here. Be carefull to select good pin to use or crash
#define THUMB      3
#define INDEX      5
#define MIDDLE     9
//#define RING       10
//#define PINKY      11 

uint32_t PWM_Pin[]       = { THUMB, INDEX, MIDDLE }; // add PINKY if pin 11 works
#define NUM_OF_PINS     3

// You can assign any interval for any timer here, in Hz
float PWM_Freq1   = 50.0f;
// You can assign any interval for any timer here, in Hz
float PWM_Freq2   = 50.0f;

// You can assign any interval for any timer here, in microseconds
uint32_t PWM_Period1 = 1000000 / PWM_Freq1;
// You can assign any interval for any timer here, in microseconds
uint32_t PWM_Period2 = 1000000 / PWM_Freq2;

// You can assign any duty_cycle for any PWM here, from 0-100
float PWM_DutyCycle1  = 5.0f;
// You can assign any duty_cycle for any PWM here, from 0-100
float PWM_DutyCycle2  = 7.5f;

// Channel number used to identify associated channel
int channelNum[5];






////////////////////////////////////////////////


#define myowarePin       A0 // Analog pin connected to MyoWare sensor
#define windowSize       50 // Size of the trigger window

int triggerWindow[windowSize];
int windowIndex = 0;
int trigger;

void readSensor()
{
  int sensorValue = analogRead(myowarePin); // Read the MyoWare sensor value
  if (SERIAL_DEBUGGING) {
    Serial.println(sensorValue); // Debugging: Print the sensor value
  }

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
  if (SERIAL_DEBUGGING) {
  Serial.println(trigger) ;
  }
}


void setup()
{
  if (SERIAL_DEBUGGING) {
    Serial.begin(115200);
  }
  while (!Serial);

  delay(2000);

  MsTimer2::set(10, readSensor); // 500ms period
  MsTimer2::start();
  if (SERIAL_DEBUGGING) {
    Serial.print(F("\nStarting ISR_Changing_PWM on ")); Serial.println(BOARD_NAME);
    Serial.println(AVR_SLOW_PWM_VERSION);
    Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));
  }

  // Timer0 is used for micros(), millis(), delay(), etc and can't be used
  // Select Timer 1-2 for UNO, 1-5 for MEGA, 1,3,4 for 16u4/32u4
  // Timer 2 is 8-bit timer, only for higher frequency
  // Timer 4 of 16u4 and 32u4 is 8/10-bit timer, only for higher frequency

#if USING_HW_TIMER_INTERVAL_MS

#if USE_TIMER_1 // TODO: we are using timer 1 according to the board picked if statement at the beginning?

  ITimer1.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,

  if (ITimer1.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler))
  {
    if (SERIAL_DEBUGGING) {
     Serial.print(F("Starting  ITimer1 OK, micros() = ")); Serial.println(micros());
    }
  }
  else {
    if (SERIAL_DEBUGGING) {
      Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
   }
  }

    
#elif USE_TIMER_3

  ITimer3.init();

  if (ITimer3.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler))
  {
    if (SERIAL_DEBUGGING) {
      Serial.print(F("Starting  ITimer3 OK, micros() = ")); Serial.println(micros());
    }
  }
  else {
    if (SERIAL_DEBUGGING) {
      Serial.println(F("Can't set ITimer3. Select another freq. or timer"));
    }
  }

#endif
=
#else

#if USE_TIMER_1 // TODO: we are using timer 1 according to the board picked if statement at the beginning?

  ITimer1.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,

  if (ITimer1.attachInterrupt(HW_TIMER_INTERVAL_FREQ, TimerHandler))
  {
    if (SERIAL_DEBUGGING) {
      Serial.print(F("Starting  ITimer1 OK, micros() = ")); Serial.println(micros());
    }
  }
  else {
    if (SERIAL_DEBUGGING) {
      Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
    }
  } 
    
#elif USE_TIMER_3

  ITimer3.init();

  if (ITimer3.attachInterrupt(HW_TIMER_INTERVAL_FREQ, TimerHandler))
  {
    if (SERIAL_DEBUGGING) {
      Serial.print(F("Starting  ITimer3 OK, micros() = ")); Serial.println(micros());
    }
  }
  else {
    if (SERIAL_DEBUGGING) {
      Serial.println(F("Can't set ITimer3. Select another freq. or timer"));
    }
  }
#endif

#endif
}
// TODO: Which of these do we want to keep
#define ACTU_PWM_FREQ          63000.0f    // in hertz
#define ACTU_PWM_PERIOD        1 / ACTU_PWM_FREQ

#define RETRACTED_PULSE        2.0f
#define EXTENDED_PULSE         1.0f

#define RETRACTED_DUTY         RETRACTED_PULSE / ACTU_PWM_PERIOD   // max_duty
#define EXTENDED_DUTY          EXTENDED_PULSE / ACTU_PWM_PERIOD    // min_duty

//#define MIN_STEP_SIZE          (1.0 / 50.0)f
//#define NUM_STEPS              50

float curr_duty_cycle=5.0f; 
int curr_step=0; 
float temp_int_float=0.0f;

#define NUM_STEPS 5


void loop()
{
    // TODO mostly fixed?: weird things about this math: curr_step is an int so it might mess with stuff, we should put in our defines for this statement so we know what each thing is
  //curr_duty_cycle = ((curr_step - 0.0) * (0.1 - 0.05) / (5 - 0) + 0.05)*100;
  // TODO: check if curr_duty_cycle is correct & check to see if changing delete worked one at a time
  curr_duty_cycle = mapf(curr_step, 0, NUM_STEPS, EXTENDED_DUTY, RETRACTED_DUTY) * 100;
  if (SERIAL_DEBUGGING) {
    Serial.print(F("Using PWM Freq = ")); Serial.print(PWM_Freq1); Serial.print(F(", PWM DutyCycle = ")); Serial.println(PWM_DutyCycle1);
  }

  for (int i = 0; i < NUM_OF_PINS; i++) 
  { 
    
    // You can use this with PWM_Freq in Hz
    channelNum[i] = ISR_PWM.setPWM(PWM_Pin[i], PWM_Freq1, curr_duty_cycle);

    delay(50);
    if (SERIAL_DEBUGGING) {
      Serial.println(curr_duty_cycle);
    }
  
    ISR_PWM.deleteChannel((unsigned) channelNum[i]);
  }

  // TODO: this is a wee bit messy the below statement would do the samething, we had it like this when we had curr_step being a float and now its an int so the mod should be fine
  // this one is the one I like: check if works 
  curr_step = (1 + curr_step) % NUM_STEPS;

   //temp_int_float = (int)(1 + temp_int_float) % NUM_STEPS;
   //curr_step = temp_int_float;
}