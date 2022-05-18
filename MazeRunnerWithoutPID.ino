char test[15];
int idx;
bool flag;


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
#define m1 2
#define m2 3
#define m3 4
#define m4 7
#define enA 5
#define enB 6

// define motor speed
#define spd 110
#define spdDiff 50
#define turnSpd 130
#define spdStraight 130
void setup()
{

  // declare serial monioutputtor for debuging
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
  return result;
}
// this function controls motor speed
void motorSpeed(int left, int right)
{
  analogWrite(enA, left);
  analogWrite(enB, right);
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
  show();
}
// this function corrects robot's left deviation make it turn right
void correctLeft()
{
  motorSpeed(spd+spdDiff, spd);
  turnOnMotors();
}
// this function corrects robot's right deviation make it turn left
void correctRight()
{
//  Serial.println("I got here");
  motorSpeed(spd, spd+spdDiff);
  turnOnMotors();
}
// this function turns left
void turnLeft()
{
  motorSpeed(0, turnSpd);
  turnOnMotors();
  delay(50);
  test[idx] = 'L';
  idx++;

}
// this function turns right
void turnRight()
{
  motorSpeed(turnSpd, 0);
  turnOnMotors();
  delay(50);
  test[idx] = 'B';
  idx++;

}
// this function goes straight
void goStraight()
{
  turnOnMotors(); 
  motorSpeed(spdStraight, spdStraight); //Motor Speed 
}

// this functions goes back
void back()
{
  delay(50);
  motorSpeed(120, 120);
  digitalWrite(m1, LOW);
  digitalWrite(m2, HIGH);
  digitalWrite(m3, HIGH);
  digitalWrite(m4, LOW);
  test[idx] = 'B';
  idx++;

}
void show(){

if(!flag)
{for(int i = 0;i<500;i++){
  if(test[i] !=0)
  Serial.println(test[i]);
  else {flag = true;break;}  }
}
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
void locatePos()
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
  case B0010:
  case B0011:
    correctLeft();
    break;
  // handle correctRight
  case B1100:
  case B0100:
    correctRight();
    break;
  // handle L
  case B1000:// just left
  case B1001: // T
  case B1110: // left and straight
  case B1010:
  case B1011:
  case B1101:
    turnLeft();
    break;
    // handle S
  case B0110:
    goStraight();
    break;
  case B0111:
    goStraight();
      test[idx] = 'S';
      idx++;
    break;
  // handle R
  case B0001:
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
  delay(4);
  locatePos();
}
