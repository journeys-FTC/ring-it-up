#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     IRSeeker,       sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     Left,          tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     Right,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     Arm,           tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Lift,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     Hand,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     motorI,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    Auto_Hand,            tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.


void moveStraight(int power, int duration)
{
	motor[Right] = power;
	motor[Left] = power;
	wait1Msec(duration);
	motor[Right] = 0;
	motor[Left] = 0;
}

void move(int powerRight, int powerLeft, int duration)
{
	motor[Right] = powerRight;
	motor[Left] = powerLeft;
	wait1Msec(duration);
	motor[Right] = 0;
	motor[Left] = 0;
}
void hand (int position)
{
	servo[Auto_Hand] = position;
}

void allStop()
{
	motor[Right] = 0;
	motor[Left] = 0;
}

int find_ir(int i){
	bool stops = false;
	while (!stops)
	{
		if(SensorValue[IRSeeker]==5 )
		{
			if(i<23)
			{
				moveStraight (-50,50);
				hand(60);
				moveStraight (-50,100);
				wait10Msec(40);
				hand(180);
				stops = true;
			}
			else
			{
				hand (60);
				moveStraight (-50,100);
				wait10Msec(40);
				hand(180);
				stops = true;
			}
		}
		else
			moveStraight (-30,100);
		i= i+1;
	}

	return i;
}

void move_past_buckets(int i){
	//	string temp = i;
	//eraseDisplay();
	//nxtDisplayCenteredBigTextLine( 4,temp);
	//wait10Msec(10000);
	string print;
	print = i;
	nxtDisplayCenteredTextLine(0,print);
	wait10Msec(400);

	int n=0;
	while(26-i > n)
	{
		moveStraight (-30,100);
		n= n+1;
	}
	{
		moveStraight (-50,350);
		move (50,-50,600);
		moveStraight (-50,1100);
		move (50,-50,700);
		moveStraight (-50,1900);
	}

}



task main()
{

	waitForStart(); // Wait for the beginning of autonomous phase.
	int i=0;
	i = find_ir(i);
	move_past_buckets(i);
}
