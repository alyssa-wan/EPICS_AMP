#include "AVR_PWM.h"

#define THUMB      3
#define INDEX      5
#define MIDDLE     9
#define RING       10
// #define PINKY      11 

/*
T = 1/f = 20ms.
retracted duty cycle = 2/20
extended duty cycle = 1/20
*/

#define ACTU_PWM_FREQ          63000.0f    // in hertz
#define ACTU_PWM_PERIOD        1 / ACTU_PWM_FREQ

#define RETRACTED_PULSE        2.0f
#define EXTENDED_PULSE         1.0f

#define RETRACTED_DUTY         RETRACTED_PULSE / ACTU_PWM_PERIOD   // max_duty
#define EXTENDED_DUTY          EXTENDED_PULSE / ACTU_PWM_PERIOD    // min_duty

#define MIN_STEP_SIZE          (1.0 / 50.0)f
#define NUM_STEPS              50

float curr_step = 0.0;
float curr_duty_cycle = 0.0;

uint32_t PWM_Pins[]       = { THUMB, INDEX, MIDDLE, RING }; // add PINKY if pin 11 works

#define NUM_OF_PINS            ( sizeof(PWM_Pins) / sizeof(PWM_Pins[0]) )

AVR_PWM* PWM_Instance[NUM_OF_PINS];
int temp_int_float = 0;

void setup() {
  // put your setup code here, to run once:
  // Attach OC0A to pwn pin
  Serial.begin(9600);

  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    PWM_Instance[index] = new AVR_PWM(PWM_Pins[index], ACTU_PWM_FREQ, EXTENDED_DUTY);

    if (PWM_Instance[index])
    {
      PWM_Instance[index]->setPWM();
    }
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  // curr_duty_cycle = map(curr_step, 0, NUM_STEPS, EXTENDED_DUTY, RETRACTED_DUTY);
  curr_duty_cycle = (curr_step - 0) * (0.1 - 0.05) / (50 - 0) + 0.05;
  for (int i = 0; i < NUM_OF_PINS; i++) { 
    PWM_Instance[i]->setPWM(PWM_Pins[i], ACTU_PWM_FREQ, curr_duty_cycle);
  }
  
  Serial.println(curr_duty_cycle);

  temp_int_float = (1 + temp_int_float) % NUM_STEPS;
  curr_step = temp_int_float;
  delay(200);
}
