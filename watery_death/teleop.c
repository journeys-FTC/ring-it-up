#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C2_1,     shoulderJoint, tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     spear,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     leftFrontPair, tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     rightFrontPair, tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_1,     leftRear,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     rightRear,     tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S1_C1_1,    handJoint,            tServoStandard)
#pragma config(Servo,  srvo_S1_C1_2,    irseekerServo,        tServoStandard)
#pragma config(Servo,  srvo_S1_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

///////////////////////////////////////////////////////////////////////////////////////
//                                                                                   //
//                                  Teleop                                           //
//                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "..\includes\AvoidWierd.h"

//***********************************************************//
//                       Variables                           //
//***********************************************************//

int maxHandValue = 255;
int minHandValue = 50;

int scoringHand = 100;
int packedHand = 205;

int currentShoulderEncoder = 0;
int tempShoulderEncoder = 0;

//***********************************************************//
//                         Methods                           //
//***********************************************************//

void all_stop(){
	motor[leftFrontPair] = 0;
	motor[leftRear] = 0;
	motor[rightFrontPair] = 0;
	motor[rightRear] = 0;
	//motor[shoulderJoint] = 0;
	//servo[ramp] = 128;
}

void drive(int ycord,int xcord, int maxVal){
	int turningVal = returnValueMotor(xcord, maxVal);
	int motorVal = returnValueMotor(ycord, maxVal);

	motor[leftRear] = motorVal - (2*turningVal);
	motor[leftFrontPair] = motorVal - (2*turningVal);

	motor[rightRear] = motorVal + (2*turningVal);
	motor[rightFrontPair] = motorVal + (2*turningVal);
}

void spearMovement(int powerVal){
	motor[spear] = powerVal;
}

void shoulderMovement(int ycord){
	int maxVal = 40;
	motor[shoulderJoint] = returnValueMotor(ycord, maxVal);
	//currentShoulderEncoder = nMotorEncoder[shoulderJoint];
	//if (currentShoulderEncoder != tempShoulderEncoder){
	//	writeDebugStreamLine("shoulder encoder current value: %d", currentShoulderEncoder);
	//	tempShoulderEncoder = currentShoulderEncoder;
	//}
	return;
}

void handMovement(int dPad){
	int currentPosition = ServoValue[handJoint];
	int newPosition = currentPosition;

	if (dPad == 0){
		if ((currentPosition - 5) > minHandValue){
			newPosition = currentPosition - 5;
		}
	}
	else if (dPad == 4){
		if ((currentPosition + 5) < maxHandValue){
			newPosition = currentPosition + 5;
		}
	}
	servo[handJoint] = newPosition;
}

void fold_arm(bool isFold){
	// if isFold is true, the arm will return to folded position
	// otherwise it will deploy to scoring position

	all_stop();
	if (isFold){
		servo[handJoint] = packedHand;
		while (nMotorEncoder[shoulderJoint] < -500){
			motor[shoulderJoint] = 30;
		}
		motor[shoulderJoint] = 0;
		writeDebugStreamLine("arm is folded at: %d", nMotorEncoder[shoulderJoint]);
		return;
	}
	else{
		while (nMotorEncoder[shoulderJoint] > -2300){
			motor[shoulderJoint] = -30;
		}
		motor[shoulderJoint] = 0;
		writeDebugStreamLine("arm is halfway at: %d", nMotorEncoder[shoulderJoint]);
		servo[handJoint] = scoringHand;

		while (nMotorEncoder[shoulderJoint] > -4500){
			motor[shoulderJoint] = -30;
		}
		motor[shoulderJoint] = 0;
		writeDebugStreamLine("arm is extended at: %d", nMotorEncoder[shoulderJoint]);

		return;
	}
}

//***********************************************************//
//                      User Control                         //
//***********************************************************//

task main()
{
	waitForStart();
	//servoChangeRate[handJoint] = 10;

	nMotorEncoder[shoulderJoint] = -500;
	writeDebugStreamLine("shoulder encoder set to: %d", nMotorEncoder[shoulderJoint]);


	int maxVal = 60;

	while (true){

		getJoystickSettings(joystick);

		int cont1_left_yval = avoidWeird(joystick.joy1_y1, 20); //y coordinate for the left joystick on controller 1
		int cont1_left_xval = avoidWeird(joystick.joy1_x1, 75); //x coordinate for the left joystick on controller 1
		int cont1_right_yval = avoidWeird(joystick.joy1_y2, 20);
		int cont1_dPad = joystick.joy1_TopHat; //Value of the dPad for controller 2

		//
		//------------Buttons-------------------
		//

		//Buttons (2,4) that control the spear movement -- will be joystick 2
		if (joy1Btn(4) == 1){
			spearMovement(20);
		}
		if (joy1Btn(2) == 1){
			spearMovement(-20);
		}
		if (joy1Btn(2) != 1 && joy1Btn(4) != 1){
			spearMovement(0);
		}

		//Buttons (5,6) that execute methods for the arm
		if (joy1Btn(6) == 1){
			fold_arm(false);
		}
		if (joy1Btn(5) == 1){
			fold_arm(true);
		}

		//Holding Button 1 will increase the speed of the robot
		if (joy1Btn(1) == 1){
			maxVal = 100;
		}
		if (joy1Btn(1) != 1){
			maxVal = 60;
		}

		//Button 8 resets the encoder value to 0
		if (joy1Btn(8) == 1){
			nMotorEncoder[shoulderJoint] = 0;
			currentShoulderEncoder = nMotorEncoder[shoulderJoint];
			tempShoulderEncoder = currentShoulderEncoder;
			writeDebugStreamLine("encoder reset to: %d", currentShoulderEncoder);
		}

		//Button 3 will pack the IRSensor
		if (joy1Btn(3) == 1){
			servo[irseekerServo] = 250;
		}

		drive(cont1_left_yval, cont1_left_xval, maxVal);
		shoulderMovement(cont1_right_yval);
		handMovement(cont1_dPad);
	}
}
