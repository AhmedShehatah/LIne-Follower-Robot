// sensors declaration
/*
 * sensorLeft for tracing if the robot has to turn left
 * sensorCorrectLeft corrects robot's path if it deviates left from the line
 * sensorCorrectRight corrects robot's path if it deviates right from the line
 * sensorBack put to stop the robot at the end of the path
 */
#define sensorLeft 12
#define sensorCorrectLeft 11
#define sensorCorrectRight 10
#define sensorRight 9
#define sensorBack 8

// motor inputs declaration
#define m1 0
#define m2 1
#define m3 2
#define m4 3
#define enA 5
#define enB 6

void setup()
{

  // declare serial monitor for debuging
  Serial.begin(9600);

  // declare motor pins mode
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(m4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  // declare sensors pins mode
  pinMode(sensorLeft, INPUT);
  pinMode(sensorCorrectLeft, INPUT);
  pinMode(sensorCorrectRight, INPUT);
  pinMode(sensorRight, INPUT);
  pinMode(sensorBack, INPUT);
}

/*
 *
 * getSensorsRead
 * this function gets sensors read and represents it in
 * a binary number each bit represents a sensor read 0 or 1
 *
 */
int getSensorsRead()
{
  // final result of readings
  int result = B0000;

  int sensorsRead[] = {
      sensorLeft,
      sensorCorrectLeft,
      sensorCorrectRight,
      sensorRight};
  // fill the array with sensors reads
  for (int i = 0; i < 4; i++)
    sensorsRead[i] = digitalRead(12 - i);

  for (int i = 0; i < 4; i++)
    if (sensorsRead[i] == HIGH)
      result += 1 << (3 - i);

  // check shifting
  for (int i = 0; i < 4; i++)
    Serial.print(sensorsRead[i]);
  Serial.println("");
  Serial.println(result);

  return result;
}
// this function controls motor speed
void motorSpeed(int left, int right)
{
  analogWrite(enA, right);
  analogWrite(enB, left);
}
void turnOnMotors()
{
  digitalWrite(m1, HIGH);
  digitalWrite(m2, LOW);
  digitalWrite(m3, HIGH);
  digitalWrite(m4, LOW);
}
// this function stops the robot at the end of the path
void stop()
{
  motorSpeed(0, 0);
  digitalWrite(m1, LOW);
  digitalWrite(m2, LOW);
  digitalWrite(m3, LOW);
  digitalWrite(m4, LOW);
}
// this function corrects robot's right deviation make it turn left
void correctLeft()
{
  motorSpeed(140, 100);
  turnOnMotors();
}
// this function corrects robot's keft deviation make it turn right
void correctRight()
{
  motorSpeed(100, 140);
  turnOnMotors();
}
// this function turns left
void turnLeft()
{
  motorSpeed(255, 0);
  turnOnMotors();
  delay(30);
}
// this function turns right
void turnRight()
{
  motorSpeed(0, 255);
  turnOnMotors();
  delay(30);
}
// this function goes straight
void goStraight()
{
  motorSpeed(140, 140);
  turnOnMotors();
}

// this functions goes back
void back()
{
  motorSpeed(255, 255);
  digitalWrite(m1, HIGH);
  digitalWrite(m2, LOW);
  digitalWrite(m3, LOW);
  digitalWrite(m4, HIGH);
}
/**
 * jsutifyPos takes sensors read and corrects robot path
 * using LSRB Algorithm
 * Do the following in order
 * L always take LEFT if possible
 * S always take STRAIGHT if LEFT is not possible
 * R always take RIGHT if STRAIGHT isn't possible
 * B always take BACK if RIGHT isn't possible
 *
 */
void justifyPos()
{
  int pos = getSensorsRead();
  int backRead = digitalRead(sensorBack);
  switch (pos)
  {
    // handle stop
  case B1111:
    if (backRead)
      stop();
    break;
  // handle correctLeft
  case B0100:
    correctLeft();
    break;
  // handle correctRight
  case B0010:
    correctRight();
  // handle L
  case B1000:
  case B1100:
  case B1110:
    turnLeft();
    break;
    // handle S
  case B0110:
    goStraight();
    break;
  // handle R
  case B0001:
  case B0011:
  case B0111:
    turnRight();
    break;
  // handle B
  case B0000:
    back();
    break;
  }
}

void loop()
{
  delay(10);
  justifyPos();
}