/****************************************************************************************************************************
  ISR_Changing_PWM.ino
  For AVR-based boards  (UNO, Nano, Mega, 32U4, 16U4, etc. )
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/AVR_Slow_PWM
  Licensed under MIT license
  
  TCNTx - Timer/Counter Register. The actual timer value is stored here.
  OCRx - Output Compare Register
  ICRx - Input Capture Register (only for 16bit timer)
  TIMSKx - Timer/Counter Interrupt Mask Register. To enable/disable timer interrupts.
  TIFRx - Timer/Counter Interrupt Flag Register. Indicates a pending timer interrupt.

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one  RP2040-based timer and avoid conflicting with other cores' tasks.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.
*****************************************************************************************************************************/
// TODO: we can probably delete the above we're changing it enough

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
int channelNum;

////////////////////////////////////////////////

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  delay(2000);

  Serial.print(F("\nStarting ISR_Changing_PWM on ")); Serial.println(BOARD_NAME);
    Serial.println(AVR_SLOW_PWM_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

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
    Serial.print(F("Starting  ITimer1 OK, micros() = ")); Serial.println(micros());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
    
#elif USE_TIMER_3

  ITimer3.init();

  if (ITimer3.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler))
  {
    Serial.print(F("Starting  ITimer3 OK, micros() = ")); Serial.println(micros());
  }
  else
    Serial.println(F("Can't set ITimer3. Select another freq. or timer"));

#endif
=
#else

#if USE_TIMER_1 // TODO: we are using timer 1 according to the board picked if statement at the beginning?

  ITimer1.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,

  if (ITimer1.attachInterrupt(HW_TIMER_INTERVAL_FREQ, TimerHandler))
  {
    Serial.print(F("Starting  ITimer1 OK, micros() = ")); Serial.println(micros());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
    
#elif USE_TIMER_3

  ITimer3.init();

  if (ITimer3.attachInterrupt(HW_TIMER_INTERVAL_FREQ, TimerHandler))
  {
    Serial.print(F("Starting  ITimer3 OK, micros() = ")); Serial.println(micros());
  }
  else
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

float curr_duty_cycle=5.0f; 
int curr_step=0; 
float temp_int_float=0.0f;

#define NUM_STEPS 5

void loop()
{
    // TODO: weird things about this math: curr_step is an int so it might mess with stuff, we should put in our defines for this statement so we know what each thing is
  curr_duty_cycle = ((curr_step - 0.0) * (0.1 - 0.05) / (5 - 0) + 0.05)*100;
  Serial.print(F("Using PWM Freq = ")); Serial.print(PWM_Freq1); Serial.print(F(", PWM DutyCycle = ")); Serial.println(PWM_DutyCycle1);

  for (int i = 0; i < NUM_OF_PINS; i++) { 
    // You can use this with PWM_Freq in Hz
    channelNum = ISR_PWM.setPWM(PWM_Pin[i], PWM_Freq1, curr_duty_cycle);

    delay(50);
    Serial.println(curr_duty_cycle);

   ISR_PWM.deleteChannel((unsigned) channelNum);
    }

  // TODO: this is a wee bit messy the below statement would do the samething, we had it like this when we had curr_step being a float and now its an int so the mod should be fine
  // this one is the one I like:
  curr_step = (1 + curr_step) % NUM_STEPS;

   //temp_int_float = (int)(1 + temp_int_float) % NUM_STEPS;
   //curr_step = temp_int_float;
}