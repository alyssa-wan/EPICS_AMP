#define WRITS_PIN 3
#define PINKY_PIN 5
#define RING_PIN 6
#define MIDDLE_PIN 9
#define POINTER_PIN 10
#define THUMB_PIN 11


void setup()
{
  pinMode(WRIST_PIN, OUTPUT);
  pinMode(PINKY_PIN, OUTPUT);
  pinMode(RING_PIN, OUTPUT);
  pinMode(MIDDLE_PIN, OUTPUT);
  pinMode(POINTER_ACTUATOR_PIN, OUTPUT);
  pinMode(THUMB_PIN, OUTPUT);

  //Rock Paper Siscor Code
  int shoot = 0;
  randomSeed(analogRead(0));
}

void loop()
{
  int wrist_val = analogRead(A0);
  int pinky_val = analogRead(A1);
  int ring_val = analogRead(A2);
  int middle_val = analogRead(A3);
  int pointer_val = analogRead(A4);
  int thumb_val = analogRead(A5);

  //pinky movement
  if (pinky_val > )
  {
    analogWrite(PINKY_PIN, pos);
  }

  //ring finger movement
  if (ring_val > )
  {
    analogWrite(RING_PIN, pos);
  }

  //middle finger movement
  if (middle_val > )
  {
    analogWrite(MIDDLE_PIN, pos);
  }

  //pointer finger movement
  if (pointer_val > )
  {
    analogWrite(POINTER_PIN, pos);
  }

  //thumb movement
  if (thumb_val > )
  {
    analogWrite(THUMB_PIN, pos);
  }
  




  if (writst_val > //need value)  //Wrist movement
  {
    analogWrite(WRIST_PIN, pos);

    //Rock Paper Siscor
    //Counts consecutive wrist flexions
    if ((wrist_val > ) && (pinky_val > ) && (ring_val > ) && (middle_val > ) && (pointer_val > ) && (thumb_val > ))
    {
      shoot += 1;
    }
    else
    {
      shoot = 0;
    }

    //if three consecutive wrist flexions then a random gesture will be generated
    if (shoot == 3)
    {
      shoot = 0;
      int random_num = random(1, 4);

      if (random_num == 1)  //Rock
      {
        analogWrite(WRIST_PIN,);
        analogWrite(PINKY_PIN, );
        analogWrite(RING_PIN, );
        analogWrite(MIDDLE_PIN, );
        analogWrite(POINTER_PIN, );
        analogWrite(THUMB_PIN, );
      }

      if (random_num == 2)  //Paper
      {
        analogWrite(WRIST_PIN,);
        analogWrite(PINKY_PIN, );
        analogWrite(RING_PIN, );
        analogWrite(MIDDLE_PIN, );
        analogWrite(POINTER_PIN, );
        analogWrite(THUMB_PIN, );
      }

      if (random_num == 3)  //Siscor
      {
        analogWrite(WRIST_PIN,);
        analogWrite(PINKY_PIN, );
        analogWrite(RING_PIN, );
        analogWrite(MIDDLE_PIN, );
        analogWrite(POINTER_PIN, );
        analogWrite(THUMB_PIN, );
      }

      delay(3000)  //this position will be held for 5 seconds
    }
  }



}

