#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     shoulderJoint, tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     rightFrontPair, tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     leftFrontPair, tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_1,     rightRear,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     leftRear,      tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S1_C4_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_2,    handJoint,            tServoStandard)
#pragma config(Servo,  srvo_S1_C4_3,    ramp,                 tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"		//Include file to "handle" the Bluetooth messages.
#include "..\includes\AutonomousFunctions.h"

task main()
{
	waitForStart();

	moveStraight(50,1630);
	move(-50,50,390);
	moveArm(-75,700);
	moveHand(160);
	moveArm(-75,900);
	moveHand(140);
	moveStraight(30,800);
	moveArm(-75,700);
	wait1Msec(1000);
	moveStraight(-30,500);
	move(50,-50,500);
	moveStraight(-30,1650);
	moveArm(75,2000);

	while (true)
	{}
}
