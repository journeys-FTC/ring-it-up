///////////////////////////////////////////////////////////////
//                                                           //
//                      Basic Functions                      //
//                        for Movement                       //
//                                                           //
///////////////////////////////////////////////////////////////


void moveStraight(int power, int duration)
{
  motor[rightSide] = power;
  motor[leftSide] = power;
  wait1Msec(duration);
  motor[rightSide] = 0;
  motor[leftSide] = 0;
}

void move(int powerRight, int powerLeft, int duration)
{
  motor[rightSide] = powerRight;
  motor[leftSide] = powerLeft;
  wait1Msec(duration);
  motor[rightSide] = 0;
  motor[leftSide] = 0;
}

void allStop()
{
  motor[rightSide] = 0;
  motor[leftSide] = 0;
  motor[shoulderJoint] = 0;
  motor[elbowJoint] = 0;
  motor[ballHopper] = 0;
}

void deployHopper(bool isUp, int power, int duration)
{
  if (isUp)
  {
    motor[ballHopper] = power;
    wait1Msec(duration);
  }
  if (isUp == false)
  {
    motor[ballHopper] = -1*power;
    wait1Msec(duration);
  }
  motor[ballHopper] = 0;
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

void deployArm()
{
  deployHopper(false,10,500);
  motor[elbowJoint] = 20;
  wait1Msec(500);
  motor[elbowJoint] = 0;
  deployHopper(false,20,500);
  motor[ballHopper] = 0;
  motor[shoulderJoint] = -20;
  wait1Msec(400);
  motor[shoulderJoint] = 0;
}


void outer(bool isBlue)
{
  if (isBlue)
  {
    moveStraight(10,5000);
    deployArm();
    wait10Msec(75);
    move(-50,50, 1000);
    moveStraight(10,500);
    move(50,-50, 1000);
    deployHopper(true,30,500);
    wait10Msec(75);
    moveStraight(20,5000);
  }
  else
  {
    moveStraight(10,5000);
    deployArm();
    wait10Msec(75);
    move(50,-50, 1000);
    moveStraight(10,500);
    move(-50,50, 1000);
    deployHopper(true,30,500);
    wait10Msec(75);
    moveStraight(20,5000);
  }
}


void hit_crates(bool isBlue)
{
  if (isBlue)
  {
    moveStraight(10,5000);
    deployArm();
    wait10Msec(75);
    move(-50,50,750);
    moveStraight(50,1000);
  }
  else
  {
    moveStraight(10,5000);
    deployArm();
    wait10Msec(75);
    move(50,-50,750);
    moveStraight(50,1000);
  }
}

void hit_bowling_ball(bool isBlue)
{
  if (isBlue)
  {
    move(-50,50,750);
    moveStraight(50,5000);
  }
  else
  {
    move(50,-50,750);
    moveStraight(50,5000);
  }

}

void bowling_ball(bool isBlue)
{
  hit_crates(isBlue);
  hit_bowling_ball(isBlue);
}




