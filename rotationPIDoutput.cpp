#include <WPILib.h>
#include "rotationPIDoutput.h"

rotationPIDoutput::rotationPIDoutput(UINT8 leftFrontTalonPort, UINT8 leftRearTalonPort, UINT8 rightFrontTalonPort, UINT8 rightRearTalonPort){
	LeftFrontTalon = new Talon(leftFrontTalonPort);
	LeftRearTalon = new Talon(leftRearTalonPort);
	RightFrontTalon = new Talon(rightFrontTalonPort);
	RightRearTalon = new Talon(rightRearTalonPort);
	Drivetrain = new RobotDrive(LeftFrontTalon, LeftRearTalon, RightFrontTalon, RightRearTalon);
}

void rotationPIDoutput::PIDWrite(float rotationValue){
	Drivetrain->MecanumDrive_Cartesian(0, 0, rotationValue);
}
