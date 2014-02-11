#ifndef ROTATIONPIDOUTPUT_H
#define ROTATIONPIDOUTPUT_H

#include <WPILib.h>

class rotationPIDoutput : public PIDOutput{

	public:
		rotationPIDoutput(RobotDrive *Drivetrain);
		void GetJoystickXY(float x, float y);
		void PIDWrite(float rotationValue);

	private:
		float xValue;
		float yValue;
		RobotDrive *Drivetrain;
};

#endif
