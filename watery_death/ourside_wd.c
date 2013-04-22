#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     HTSMUX,         sensorI2CCustom)
#pragma config(Sensor, S3,     ,               sensorHiTechnicTouchMux)
#pragma config(Motor,  mtr_S1_C2_1,     shoulderJoint, tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     spear,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     rightFrontPair, tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     leftFrontPair, tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_1,     rightRear,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     leftRear,      tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S1_C1_1,    handJoint,            tServoStandard)
#pragma config(Servo,  srvo_S1_C1_2,    irseekerServo,        tServoStandard)
#pragma config(Servo,  srvo_S1_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "..\includes\rdpartyrobotcdr-3.3.1\drivers\hitechnic-sensormux.h"
#include "..\includes\rdpartyrobotcdr-3.3.1\drivers\lego-light.h"
#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.

#include "..\includes\autoFunctions-rings.h"

// Name the light sensors
const tMUXSensor right_light = msensor_S2_1;
const tMUXSensor middle_light = msensor_S2_2;
const tMUXSensor left_light = msensor_S2_3;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//																Global Variables
//
// Variables that will be used by the code from outside functions.
//
// Global Variables:
//
//		1. name: black_threshold
//			 description: the value of the color black
//		   function: values lower than this number are the black color of the wood under the scoring
//					board
//
//		2. name: white_threshold
//			 description: the value of the color white
//			 function: values higher than this number are the white color of the tape on the scoring
//					board
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

int black_threshold; //Calibrate this at the beginning of autonomous
int white_threshold; //Set this as black_threshold + 8

// int white_threshold = 45;

// Set up light sensor variables
int left_nrm = 0;
int middle_nrm = 0;
int right_nrm = 0;

// Set up temp vals to decrease screen flicker
int temp_left_nrm = 0;
int temp_middle_nrm = 0;
int temp_right_nrm = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    initializeRobot
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

void initializeRobot()
{

	// Set light sensors to active
	LSsetActive(left_light);
	LSsetActive(middle_light);
	LSsetActive(right_light);

	// Initialize the encoder
	nMotorEncoder[shoulderJoint] = 0;
	//nMotorEncoder[spear] = 0;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                         Main Task
//
// The following is the main code for the autonomous robot operation.
//
// This code will cause the robot to score on the middle ring on our side of the field.
//
// Steps:
//
//		1. Move forward, turn, move forward, and turn
//				a. This is to get the robot to be perpendicular to the white lines on our side
//
//		2. Find the line
//				a. The robot will move forward until it first detects the line.
//
//		3. Turn 90 degrees
//				a. As the sensors are on the front of the robot, turning 90 degrees from when the sensors
//					 first find the line will not put the robot on that line.
//				b. After finding the line, the robot needs to move forward for a distance of half its length,
//					 and then turn 90 degrees.
//
//		4. Follow the line until touch sensor is triggered
//				a. The robot will move forward slowly, periodically checking that is still on the line.
//				b. The sensors will be configured such that the left one is always on black, the middle one is
//					 always on white, and the right one is always on black.
//				c. Using these configurations, corrections will be calculated based on the positions of all
//					 three sensors in regards to the white line.
//							1. Left on white, middle on white (or black), right on black: turn toward the left
//							2. Left on black, middle on white (or black), right on white: turn toward the right
//							3. Left on black, middle on white, right on black: move straight
//
//		4. More later...(score the ring)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

task main()
{
	initializeRobot();
	waitForStart(); // Wait for the beginning of autonomous phase.
	servo[handJoint] = 240;
	wait1Msec(100);

	black_threshold = LSvalNorm(middle_light);
	white_threshold = black_threshold + 3;

	writeDebugStreamLine("black_threshold: %d", black_threshold);
	writeDebugStreamLine("white_threshold: %d", white_threshold);

	//
	// Move to other side of field
	//

	moveStraight(50,100);
	move(-50,50,650);
	moveStraight(50,1300);
	move(50,-50,450);
	moveStraight(50,450);

	//
	// Find the line
	//
	// Continue forward until the middle sensor detects the white line
	//
	writeDebugStreamLine("middle light sensor value going into first loop: %d" , LSvalNorm(middle_light));

	while (LSvalNorm(middle_light) < white_threshold){

		//!--Code for debugging
		middle_nrm = LSvalNorm(middle_light);
		if (middle_nrm != temp_middle_nrm) {
			writeDebugStreamLine("middle light sensor value: %d", middle_nrm);
			temp_middle_nrm = middle_nrm;
		}
		//--!

		setAllMotorVals(10);
	}
	allStop();
	wait1Msec(100);

	//
	// Turn 90 degrees
	//
	// Move forward a little more and then turn until the middle sensor detects the white line
	//

	bool isLeft = true; // If this boolean is true, the 90 degree turn is toward the left

	moveStraight(30,350);
	//moveStraight(15,1000); // with long arm

	// Make a variable that counts the number of times through the loop. If it goes above a certain
	// threshold, assume the robot is stuck on the edge of the wood and increase the power
	int counter1 = 0;

	while (LSvalNorm(middle_light) < white_threshold){

		//!--Code for debugging
		middle_nrm = LSvalNorm(middle_light);
		if (middle_nrm != temp_middle_nrm) {
			writeDebugStreamLine("left light sensor value: %d", middle_nrm);
			temp_middle_nrm = middle_nrm;
		}
		//--!

		if (isLeft){
			if (counter1 > 500){
				setDriveMotorVals(20,-20);
			}
			else{
				setDriveMotorVals(15,-15);
			}
		}
		else{
			if (counter1 > 100){
				setDriveMotorVals(-20,20);
			}
			else{
				setDriveMotorVals(-15,15); // with long arm
			}
		}
		counter1 += 1;
	}

	//
	// Follow the line until touch sensor is triggered
	//
	//	1. Left on white, middle on white (or black), right on black: turn toward the left
	//	2. Left on black, middle on white (or black), right on white: turn toward the right
	//	3. Left on black, middle on white, right on black: move straight
	//	4. Left on black, middle on black, right on black: lost...
	//

	// Set up touch sensor variables
	int nButtonsMask;
	bool isTouch = false; // Boolean indicating if a touch sensor has been pressed.
												// If one has, this changes to true.

	// Make a variable that counts the number of times through the loop. If it goes above a certain
	// threshold, assume the robot is stuck on the edge of the wood and increase the power
	int counter2 = 0;

	//deploy the spear
	deploySpear(true);

	while (isTouch == false){

		writeDebugStreamLine("-----------------counter2 val: %d", counter2);

		// Read light sensor values
		left_nrm = LSvalNorm(left_light);
		middle_nrm = LSvalNorm(middle_light);
		right_nrm = LSvalNorm(right_light);

		//!--Code for debugging
		//if (left_nrm != temp_left_nrm) {
			writeDebugStreamLine("left light sensor value: %d", left_nrm);
			//temp_left_nrm = left_nrm;
		//}
		//if (middle_nrm != temp_middle_nrm) {
			writeDebugStreamLine("middle light sensor value: %d", middle_nrm);
			//temp_middle_nrm = middle_nrm;
		//}
		//if (right_nrm != temp_right_nrm) {
			writeDebugStreamLine("middle light sensor value: %d", right_nrm);
			//temp_right_nrm = right_nrm;
		//}
		//--!

		if (left_nrm < black_threshold){
			// left sensor is black

			if (middle_nrm < black_threshold){
				// middle sensor is black

				if (right_nrm > black_threshold){
					// right sensor is white or gray
					// turn left
					writeDebugStreamLine("Case 1: turned left");

					if (counter2 > 600){
						setDriveMotorVals(40,0);
					}
					else{
						setDriveMotorVals(30,0);
					}
				}
				else{
					// right sensor is black
					// lost, so just turn right a lot

					setDriveMotorVals(30,-30);
				}
			}

			else{
				// middle sensor is not black

				if (middle_nrm > white_threshold){
					// middle sensor is white
					// assume right sensor is black
					// move straight

					writeDebugStreamLine("Case 2: went straight");

					if (counter2 > 600){
						setAllMotorVals(30);
					}

					else{
						setAllMotorVals(20);
					}
				}
				else{
					// middle sensor is gray
					// assume right sensor is gray
					// turn left

					writeDebugStreamLine("Case 3: turned left");

					if (counter2 > 600){
						setDriveMotorVals(40,0);
					}
					else{
						setDriveMotorVals(30,0);
					}
				}
			}
		}
		else{
			// left sensor is not black

			if (left_nrm > white_threshold){
				// left sensor is white
				// assume middle sensor is black
				// assume the right sensor is black too
				// turn right

				writeDebugStreamLine("Case 4: turned right");

				if (counter2 > 600){
					setDriveMotorVals(0,40);
				}
				else{
					setDriveMotorVals(0,30);
				}
			}
			else{
				// left sensor is gray
				// middle sensor is gray
				// assume right sensor is black
				// turn right

				writeDebugStreamLine("Case 5: turned right");

				if (counter2 > 600){
					setDriveMotorVals(0,40);
				}
				else{
					setDriveMotorVals(0,30);
				}
			}
		}

		counter2 += 1;

		// Read touch sensor values
		nButtonsMask = SensorValue[S3];

		if (nButtonsMask & 0x01)
			isTouch = true;
		if (nButtonsMask & 0x02)
			isTouch = true;
	}
	allStop();
	wait1Msec(1000);

	//
	// More later....scoring stuff
	//
	//	1. Move backward a little
	//	2. Unfold arm
	//	3. Move forward a little
	//	4. Move arm down to score
	//	5. Back up
	//	6. Reset arm
	//	7. Move toward our rings?
	//


	// back up a little
	moveStraight(-20,800);

	// unfold arm
	fold_arm(false);

	// move forward a little
	moveStraight(20,600);

	// move arm down
	motor[shoulderJoint] = -50;
	wait1Msec(800);
	motor[shoulderJoint] = 0;

	// back up
	moveStraight(-40,500);

	// reset arm
	fold_arm(true);

	//retract spear
	deploySpear(false);

	while (true){
		return;
	}
}
