///////////////////////////////////////////////////////////////
//                                                           //
//                      Basic Functions                      //
//                        for Movement                       //
//                                                           //
///////////////////////////////////////////////////////////////


void moveStraight(int power, int duration)
{
  motor[rightRear] = power;
  motor[rightFrontPair] = power;
  motor[leftRear] = power;
  motor[leftFrontPair] = power;
  wait1Msec(duration);
  motor[rightRear] = 0;
  motor[rightFrontPair] = 0;
  motor[leftRear] = 0;
  motor[leftFrontPair] = 0;
}

void move(int powerRight, int powerLeft, int duration)
{
  motor[rightRear] = powerRight;
  motor[rightFrontPair] = powerRight;
  motor[leftRear] = powerLeft;
  motor[leftFrontPair] = powerLeft;
  wait1Msec(duration);
  motor[rightRear] = 0;
  motor[rightFrontPair] = 0;
  motor[leftRear] = 0;
  motor[leftFrontPair] = 0;
}

void moveArm (int power, int duration)
{
	motor[shoulderJoint] = power;
	wait1Msec(duration);
	motor[shoulderJoint] = 0;
}

void moveHand(int position)
{
	servo[handJoint] = position;
}

void allStop()
{
  motor[rightRear] = 0;
  motor[rightFrontPair] = 0;
  motor[leftRear] = 0;
  motor[leftFrontPair] = 0;
  motor[shoulderJoint] = 0;
}

bool buttonValid(int btn)
{
  if (btn == 1)
  {
    nxtDisplayCenteredBigTextLine(3, "RED OUTER?");
    int btn2 = nNxtButtonPressed;
    while (btn2 == -1)
    {
      btn2 = nNxtButtonPressed;
    }
    if (btn2 == 0)
    {
      return true;
    }
  }
  if (btn == 2)
  {
    nxtDisplayCenteredBigTextLine(3, "BLUE OUTER?");
    int btn3 = nNxtButtonPressed;
    while (btn3 == -1)
    {
      btn3 = nNxtButtonPressed;
    }
    if (btn3 == 0)
    {
      return true;
    }
  }
  return false;
}
