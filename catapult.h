#ifndef CATAPULT_H
#define CATAPULT_H

#include <WPILib.h>

class catapult{
	
	public:
		catapult(UINT8 talonOnePort, UINT8 talonTwoPort, UINT8 encoderPortA, UINT8 encoderPortB, UINT8 limitSwitchPort);
		
		void ReInit();
		
		//Fires the catapult, automatically cycles back to bottom position after firing
		void Fire();
		
		//void SetAutonomousLowering();
		
		//Sets the number of encoder "clicks" at which power to the catapult is cut
		void SetStoppingPoint(int stoppingClicks);
		
		//Sets the power at which the motors are run at when firing
		void SetMotorPower(float motorPower);
		
		//Returns the current encoder count
		int GetEncoderCount();
		
		//Returns the current shooter state
		int GetState();
		
		//Resets the encoder
		void ResetEncoder();

		//Function to be called every loop, this is where all the "work" is actually done		
		void Idle();
		
	private:
		int stoppingClicks;
		float motorPower;
		Talon *MotorOneTalon;
		Talon *MotorTwoTalon;
		Encoder *CatapultEncoder;
		Timer *LoweringTimer;
		enum firingState_t{
			waiting,
			firing,
			lowering,
			zeroing,
			//autonomousLowering,
		};
		firingState_t firingState;
};

#endif
