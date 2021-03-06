#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S2,     HTSMUX,         sensorI2CCustom)
#pragma config(Sensor, S3,     ,               sensorHiTechnicTouchMux)
#pragma config(Motor,  mtr_S1_C1_1,     shoulderJoint, tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     ramp,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     rightFrontPair, tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     leftFrontPair, tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     rightRear,     tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     leftRear,      tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    handJoint,            tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//																Global Variables
//
// Variables that will be used by the code from outside functions.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////


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


}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//													Specific Functions for Touch Sensor
//
/////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//														Movement Functions
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

void pack_hand()
{
	motor[shoulderJoint] = 40;
	wait1Msec(1600);
	servo[handJoint] = 60;
	wait1Msec(350);
	motor[shoulderJoint] = 0;
}

void allStop()
{
	motor[rightRear] = 0;
	motor[rightFrontPair]= 0;
	motor[leftRear] = 0;
	motor[leftFrontPair] = 0;
	motor[shoulderJoint] = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                         Main Task
//
// The following is the main code for the autonomous robot operation.
//
// This code will cause the robot to move forward until one of its two touch sensors are triggered.
// This code assumes the touch sensors are connected to a touch multiplexer.
//
// Steps:
//
//		1. Move forward until touch sensor triggered
//
//		2. Stop
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

task main()
{
	initializeRobot();
	waitForStart(); // Wait for the beginning of autonomous phase.
	wait1Msec(100);

	int nButtonsMask;
	bool isTouch = false; // Boolean indicating if a touch sensor has been pressed.
												// If one has, this changes to true.

	while (isTouch == false){

		moveStraight(-15,10);

		nButtonsMask = SensorValue[S3];

		if (nButtonsMask & 0x01)
			isTouch = true;
		if (nButtonsMask & 0x02)
			isTouch = true;
	}


	while (true){
		return;
	}
}
