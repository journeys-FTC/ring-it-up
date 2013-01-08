#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S2,     IRSeeker,       sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     shoulderJoint, tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     ramp,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     rightFrontPair, tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     leftFrontPair, tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     rightRear,     tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     leftRear,      tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_2,    handJoint,            tServoStandard)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                           Autonomous Mode Code Template
//
// This file contains a template for simplified creation of an autonomous program for an TETRIX robot
// competition.
//
// You need to customize two functions with code unique to your specific robot.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    initializeRobot
//
// Prior to the start of autonomous mode, you may want to perform some initialization on your robot.
// Things that might be performed during initialization include:
//   1. Move motors and servos to a preset position.
//   2. Some sensor types take a short while to reach stable values during which time it is best that
//      robot is not moving. For example, gyro sensor needs a few seconds to obtain the background
//      "bias" value.
//
// In many cases, you may not have to add any code to this function and it will remain "empty".
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

void initializeRobot()
{
	/*
	eraseDisplay();
	string print;
	int n = 0;
	while (n<400){
	print = SensorValue[IRSeeker];
	nxtDisplayCenteredTextLine(0,print);
	wait10Msec(10);
	n = n + 1;
	}
	*/
	return;

}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                         Main Task
//
// The following is the main code for the autonomous robot operation. Customize as appropriate for
// your specific robot.
//
// The types of things you might do during the autonomous phase (for the 2008-9 FTC competition)
// are:
//
//   1. Have the robot follow a line on the game field until it reaches one of the puck storage
//      areas.
//   2. Load pucks into the robot from the storage bin.
//   3. Stop the robot and wait for autonomous phase to end.
//
// This simple template does nothing except play a periodic tone every few seconds.
//
// At the end of the autonomous period, the FMS will autonmatically abort (stop) execution of the program.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void moveStraight(int power, int duration)
{
	motor[rightRear] = power;
	motor[rightFrontPair]= power;
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
	motor[rightFrontPair]= powerRight;
	motor[leftRear] = powerLeft;
	motor[leftFrontPair] = powerLeft;
	wait1Msec(duration);
	motor[rightRear] = 0;
	motor[rightFrontPair]= 0;
	motor[leftRear] = 0;
	motor[leftFrontPair] = 0;
}
void movearm (int power, int duration)
{
	motor[shoulderJoint] = power;
	wait1Msec(duration) ;
	motor[shoulderJoint] = 0;
}

void movehand (int position)
{
	servo[handJoint] = position;
}

void allStop()
{
	motor[rightRear] = 0;
	motor[rightFrontPair]= 0;
	motor[leftRear] = 0;
	motor[leftFrontPair] = 0;
	motor[shoulderJoint] = 0;
}


task main()
{
	initializeRobot();
	waitForStart(); // Wait for the beginning of autonomous phase.
	{

		if(SensorValue[IRSeeker] == 7)	//right
		{
			//...turn left.
			moveStraight (-50,115);
			move (50,-50,456);
			movearm (75,700);
			movehand (160);
			movearm (75,700);
			movehand (130);
			moveStraight (-30,1900);
			movearm (75,750);
			moveStraight (50, 1000);
		}
		else if(SensorValue[IRSeeker] <= 5)	//left
		{
			moveStraight (-50,1900);
			move (50,-50,440);
			movearm (75,700);
			movehand (160);
			movearm (75,700);
			movehand (130);
			moveStraight (-30,450);
			movearm (75,800);
			moveStraight (50,200);
		}
		else if(SensorValue[IRSeeker] == 6)	//middle
		{
			moveStraight (-50,800);
			move (50,-50,421);
			movearm (75,700);
			movehand (160);
			movearm (75,700);
			movehand (130);
			moveStraight (-30,1350);
			movearm (75,850);
			moveStraight (50,1400);
		}
	}
	while (true)
	{
		return;
	}
}