#include "chassis.h"
#include <WPILib.h>

chassis::chassis(UINT8 leftFrontTalonPort, UINT8 leftRearTalonPort, UINT8 rightFrontTalonPort, UINT8 rightRearTalonPort){
	LeftFrontTalon = new Talon(leftFrontTalonPort);
	LeftRearTalon = new Talon(leftRearTalonPort);
	RightFrontTalon = new Talon(rightFrontTalonPort);
	RightRearTalon = new Talon(rightRearTalonPort);
	Drivetrain = new RobotDrive(LeftFrontTalon, LeftRearTalon, RightFrontTalon, RightRearTalon);
	Drivetrain->SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
	Drivetrain->SetInvertedMotor(RobotDrive::kFrontLeftMotor, false);
	Drivetrain->SetInvertedMotor(RobotDrive::kRearRightMotor, false);
	Drivetrain->SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
	chassisState = mecanum;
}

void chassis::SetJoystickData(float x, float y, float twist){
	xValue = x;
	yValue = y;
	twistValue = twist;
}

void chassis::ToggleAxisLock(){
	chassisState = axisLock;
}

void chassis::Idle(){
	switch(chassisState){
		
		case mecanum:
			if(xValue < 0) mappedX = -(xValue * xValue);
			else mappedX = xValue * xValue;
			if(yValue < 0) mappedY = -(yValue * yValue);
			else mappedY = yValue * yValue;
			if(twistValue < 0) mappedTwist = -(twistValue * twistValue);
			else mappedTwist = twistValue * twistValue;
			Drivetrain->MecanumDrive_Cartesian(mappedX, mappedY, mappedTwist, 0.0);
		break;

		case axisLock:

		break;
	}
}
