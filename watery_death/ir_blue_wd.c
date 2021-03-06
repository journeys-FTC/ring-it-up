#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     HTSMUX,         sensorI2CCustom)
#pragma config(Sensor, S3,     ,               sensorHiTechnicTouchMux)
#pragma config(Sensor, S4,     IRSEEKER,       sensorI2CCustom)
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

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "..\includes\rdpartyrobotcdr-3.3.1\drivers\hitechnic-irseeker-v2.h"
#include "..\includes\rdpartyrobotcdr-3.3.1\drivers\hitechnic-sensormux.h"
#include "..\includes\rdpartyrobotcdr-3.3.1\drivers\hitechnic-compass.h"

#include "..\includes\autoFunctions-rings.h"

// Set up compass sensor to the multiplexer
const tMUXSensor COMPASS = msensor_S2_4;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//																Global Variables
//
// Variables that will be used by the code from outside functions.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

int SERVO_OPTIMAL_POS = 85; // This is the position at which the ir servo is at the correct angle
														// for turning to score rings.

int servoValAfterIScan; // The value of the servo after the initial scan for direction

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    initializeRobot
//
// Prior to the start of autonomous mode, you may want to perform some initialization on your robot.
//
// In many cases, you may not have to add any code to this function and it will remain "empty".
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

void initializeRobot()
{
	HTMCsetTarget(COMPASS);
  nMotorEncoder[shoulderJoint] = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//													Specific Functions for IRSensor
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

int findIR (){

	//
	// This function moves the servo with the IRSensor on it to find the strongest signal from the IRBeacon.
	// It returns the final position of the irServo.
	//
	// First, it finds the general directs via a function of the IRSensor that returns a number from 0-9. The
	// sectioning is diagramed here:
	// http://www.usfirst.org/sites/default/files/uploadedFiles/Robotics_Programs/FTC/FTC_Documents/Using_the_IR_Seeker.pdf
	// Next, it determines the strongest signal within the section. Again, see the above link for a diagram.
	//
	// This is all achieved by mounting the IRSensor on a servo motor, and rotating the servo so the
	// IRSensor is facing the direction of the strongest signal strength.
	//

	//
	// Determines the general direction of the IRBeacon, and then positions the servo
	// at the lower limit of the 5 range (i.e. dir is on the edge of 5 and 6).
	//
	// 5 is straight ahead,
	// 1-4 are to the left,
	// 6-9 are to the right,
	// and 0 is broken. (well not really, but it means it couldn't find a signal
	//

	servoChangeRate[irseekerServo] = 5;
	int dir;
	int prevdir;
	dir = HTIRS2readACDir(IRSEEKER);
	writeDebugStreamLine("dir: %d", dir);
	prevdir = dir;
	while (dir <= 5){
		if ((ServoValue[irseekerServo] - 2) > 10){
			servo[irseekerServo] = ServoValue[irseekerServo] - 2;
			wait1Msec(50);
			dir = HTIRS2readACDir(IRSEEKER);
		}
		else{
			writeDebugStreamLine("servo tried to go too far toward 0");
			break;
		}
		if (dir != prevdir){
			prevdir = dir;
			writeDebugStreamLine("dir: %d", dir);
		}
	}
	while (dir > 5){
		if ((ServoValue[irseekerServo] + 2) < 245){
			servo[irseekerServo] = ServoValue[irseekerServo] + 2;
			wait1Msec(50);
			dir = HTIRS2readACDir(IRSEEKER);
		}
		else{
			writeDebugStreamLine("servo tried to go too far toward 255");
			break;
		}
		if (dir != prevdir){
			prevdir = dir;
			writeDebugStreamLine("dir: %d", dir);
		}
	}

	servoValAfterIScan = ServoValue[irseekerServo];

	//
	// Fine tune using the signal strengths within each of the five sensors on the IRSensor
	//
	servoChangeRate[irseekerServo] = 1;
	int acS1, acS2, acS3, acS4, acS5 = 0;
	int maxSensor;
	int maxServo;

  maxSensor = acS5;
  maxServo = ServoValue[irseekerServo];
  while (dir == 5){

    if (!HTIRS2readAllACStrength(IRSEEKER, acS1, acS2, acS3, acS4, acS5 ))
      break; // I2C read error occurred

    writeDebugStreamLine("strengths 1,2,3,4,5: %d, %d, %d, %d, %d", acS1, acS2, acS3, acS4, acS5);

	 	//writeDebugStreamLine("strength: %d", acS5);

	 	if (acS3 < (maxSensor-50)){
	 		break;
	 	}

	  if (acS3 > maxSensor){
	  	maxSensor = acS3;
	  	maxServo = ServoValue[irseekerServo];
	  }

	  writeDebugStreamLine("maxSensor: %d", maxSensor);

	  if ((ServoValue[irseekerServo] + 1) < 245){
			servo[irseekerServo] = ServoValue[irseekerServo] + 1;
			wait1Msec(20);
			dir = HTIRS2readACDir(IRSEEKER);
		}
	}
	servo[irseekerServo] = servoValAfterIScan;
	wait1Msec(100);
	writeDebugStreamLine("robot believes optimal position to be %d.", maxServo);
	return maxServo;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                         Main Task
//
// The following is the main code for the autonomous robot operation. Customize as appropriate for
// your specific robot.
//
// Brief description: score on the rack using an IRSensor and a compass sensor. The IRSensor is mounted
// on a servo to provide an angle for the direction of the IRBeacon in relation to the orientation of
// the robot.
//
// Steps:
//
//		1. Drive forward slightly to clear the rings
//
//		2. Find the IRBeacon with the IRSensor
//				a. Determine the angle with relation to the orientation of the robot
//
//		3. Drive forward a little and then determine the angle of the IRBeacon again
//				a. Perform a sweep with the IRSensor to find the strongest value
//
//		4. Continue step 3 until the desired angle and the angle of the IRBeacon line up
//
//		5. Turn the desired angle to line the robot up with the scoring rack (45 degrees in this case)
//
//		6. Go score...
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

task main()
{
	waitForStart();
	wait1Msec(20);
	servo[irseekerServo] = 100;
	//servo[handJoint] = 240;
	wait1Msec(100);
	int currentServoPosition = -1;

	//***** From here on, the code is specialized toward the middle and far pegs (for now...)

	// Drive forward slightly to clear rings
	moveStraight(50,1000);

	writeDebugStreamLine("Initial position for IRBeacon is: %d", HTIRS2readACDir(IRSEEKER));


	// Find the IRBeacon with the IRSensor
	currentServoPosition = findIR();

	while ((currentServoPosition < SERVO_OPTIMAL_POS + 3) && (currentServoPosition < SERVO_OPTIMAL_POS - 3)){
		// Continue moving along a straight line while the servo and its optimal position are not the same

		while (currentServoPosition < SERVO_OPTIMAL_POS - 3){
			// Move forward while the servo's position is less than optimal
			moveStraight(-15,4*currentServoPosition);
			currentServoPosition = findIR();
		}
		while (currentServoPosition > SERVO_OPTIMAL_POS + 3){
			// Move backward while the servo's position is more than optimal
			moveStraight(15,4*currentServoPosition);
			currentServoPosition = findIR();
		}
	}

	// Turn 45 degrees

	// Deploy the spear
	deploySpear(true);

	// Move forward until the touch sensor is triggered
	// Set up touch sensor variables
	int nButtonsMask;
	bool isTouch = false; // Boolean indicating if a touch sensor has been pressed.
												// If one has, this changes to true.
	while (isTouch == false){
		setAllMotorVals(20);

		if (nButtonsMask & 0x01)
			isTouch = true;
		if (nButtonsMask & 0x02)
			isTouch = true;
	}
	allStop();

	// back up a little
	moveStraight(-20,650);

	//retract spear
	deploySpear(false);

	// unfold arm
	fold_arm(false);

	// move forward a little
	moveStraight(20,300);
	wait1Msec(1000);

	// move arm down
	motor[shoulderJoint] = -20;
	wait1Msec(400);
	motor[shoulderJoint] = 0;

	// back up
	moveStraight(-40,500);

	// reset arm
	fold_arm(true);

}
