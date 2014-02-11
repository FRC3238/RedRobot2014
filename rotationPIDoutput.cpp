#include <WPILib.h>
#include "rotationPIDoutput.h"

rotationPIDoutput::rotationPIDoutput(RobotDrive *Drivetrain){
	
}

void rotationPIDoutput::GetJoystickXY(float x, float y){
	xValue = x;
	yValue = y;
}

void rotationPIDoutput::PIDWrite(float rotationValue){
	Drivetrain->MecanumDrive_Cartesian(xValue, yValue, rotationValue);
}
