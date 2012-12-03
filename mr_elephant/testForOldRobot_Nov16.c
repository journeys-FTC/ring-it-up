#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     ballHopper,    tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     hand,          tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     shoulderJoint, tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     elbowJoint,    tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     rightSide,     tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     leftSide,      tmotorNormal, openLoop, reversed)
#pragma config(Servo,  srvo_S1_C3_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_2,    leftHopper,           tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C3_3,    rightHopper,          tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

///////////////////////////////////////////////////////////////////////////////////////
//                                                                                   //
//                                  Teleop                                           //
//                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "AvoidWierd.h"

//***********************************************************//
//                       Variables                           //
//***********************************************************//

int maxHandServo = 100;
int minHandServo = 10;

//***********************************************************//
//                         Methods                           //
//***********************************************************//

void drive(int ycord,int xcord)
{
  int maxVal = 15;
  int turningVal = returnValueMotor(xcord, maxVal);
  int motorVal = returnValueMotor(ycord, maxVal);

  motor[leftSide] = motorVal + (2*turningVal);

  motor[rightSide] = motorVal - (2*turningVal);
}

void shoulderMovement(int ycord)
{
  if (ycord > 0)
  {
    int maxVal = 40;
    motor[shoulderJoint] = returnValueMotor(ycord, maxVal);
    return;
  }

  else
  {
    int maxVal = 40;
    motor[shoulderJoint] = returnValueMotor(ycord, maxVal);
    return;
  }
}

void elbowMovement(int ycord)
{
  int maxVal = 40;
  motor[elbowJoint] = returnValueMotor(ycord, maxVal);
  return;
}

void ballHopperArm (int cord)
{
  int maxVal = 15;
  motor[ballHopper] = returnValueMotor(cord,maxVal);
}

void contServos (int isIn)
{
  if (isIn == 0)
  {
    servo[rightHopper] = 254;
    servo[leftHopper] = 1;
  }
  if (isIn == 1)
  {
    servo[rightHopper] = 1;
    servo[leftHopper] = 254;
  }
  if (isIn == -1)
  {
    servo[rightHopper] = 128;
    servo[leftHopper] = 128;
  }
}

void handMovement (int dPad)
{
  if (dPad == 0)
  {
    motor[hand] = 20;
  }
  if (dPad == 4)
  {
    motor[hand] = -20;
  }
  if (dPad == -1)
  {
    motor[hand] = 0;
  }
}


//***********************************************************//
//                      User Control                         //
//***********************************************************//

task main()
{
  waitForStart();

  int isIn = -1;
  while (true)
  {
    getJoystickSettings(joystick);

    int cont1_left_yval = avoidWeird(joystick.joy1_y1, 20); //y coordinate for the left joystick on controller 1
    int cont1_left_xval = avoidWeird(joystick.joy1_x1, 75); //x coordinate for the left joystick on controller 1
    int cont1_right_yval = avoidWeird(joystick.joy1_y2, 20);
    int cont2_left_yval = avoidWeird(joystick.joy2_y1, 35); //y coordinate for the left joystick on controller 2
    int cont2_right_yval = avoidWeird(joystick.joy2_y2, 35); //y coordinate for the right joystick on controller 2
    int cont2_dPad = joystick.joy2_TopHat; //Value of the dPad for controller 2

    if (joy1Btn(4) == 1)
    {
      isIn = 0;
    }
    if (joy1Btn(2) == 1)
    {
      isIn = 1;
    }
    if (joy1Btn(6) == 1)
    {
      isIn = -1;
    }

    drive(cont1_left_yval, cont1_left_xval);
    shoulderMovement(cont2_left_yval);
    elbowMovement(cont2_right_yval);
    contServos(isIn);
    ballHopperArm(cont1_right_yval);
    handMovement(cont2_dPad);
  }
}
