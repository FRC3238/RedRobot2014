#ifndef CHASSIS_H
#define CHASSIS_H

#include <WPILib.h>
#include "rotationPIDoutput.h"

class chassis{
	
	public:
		chassis(UINT8 leftFrontTalonPort, UINT8 leftRearTalonPort, UINT8 rightFrontTalonPort, UINT8 rightRearTalonPort, UINT8 leftUltrasonicPort, UINT8 rightUltrasonicPort);

		//Sets the current joystick data
		void SetJoystickData(float x, float y, float twist);

		//Enables axis lock
		void EnableAxisLock();

		//Disables axis lock
		void DisableAxisLock();
		
		//Function to be called every loop, this is where the "work" is actually done
		void Idle();

	private:
		float xValue;
		float yValue;
		float twistValue;
		float mappedX;
		float mappedY;
		float mappedTwist;
		Talon *LeftFrontTalon;
		Talon *LeftRearTalon;
		Talon *RightFrontTalon;
		Talon *RightRearTalon;
		RobotDrive *Drivetrain;
		AnalogChannel *LeftFrontSonar;
		AnalogChannel *LeftRearSonar;
		AnalogChannel *RightFrontSonar;
		AnalogChannel *RightRearSonar;
		rotationPIDoutput *PIDOutputRotation;
		PIDController *ChassisPID;
		enum chassisState_t{
			mecanum,
			axisLock,
		};
		chassisState_t chassisState;
			
};

#endif
