#ifndef CHASSIS_H
#define CHASSIS_H

#include <WPILib.h>

class chassis{
	
	public:
		chassis(UINT8 leftFrontTalonPort, UINT8 leftRearTalonPort, UINT8 rightFrontTalonPort, UINT8 rightRearTalonPort);


		//Sets the current joystick data
		void SetJoystickData(float x, float y, float twist);

		//Toggles between standard control and axis-locked control
		void ToggleAxisLock();

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
		enum chassisState_t{
			mecanum,
			axisLock,
		};
		chassisState_t chassisState;
			
};

#endif
