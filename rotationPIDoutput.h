#ifndef ROTATIONPIDOUTPUT_H
#define ROTATIONPIDOUTPUT_H

#include <WPILib.h>
#include "chassis.h"

class rotationPIDoutput : public PIDOutput{

	public:
		rotationPIDoutput(UINT8 leftFrontTalonPort, UINT8 leftRearTalonPort, UINT8 rightFrontTalonPort, UINT8 rightRearTalonPort);
		void PIDWrite(float rotationValue);

	private:
		Talon *LeftFrontTalon;
		Talon *LeftRearTalon;
		Talon *RightFrontTalon;
		Talon *RightRearTalon;
		RobotDrive *Drivetrain;
		
};

#endif
