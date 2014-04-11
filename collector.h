#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <WPILib.h>
#include "arduinoI2C.h"
class collector{

	public:
		collector(UINT8 liftingTalonPort, UINT8 rollerTalonPort, ArduinoI2C* ballSensorPort, UINT8 upperLimitSensorPort, UINT8 lowerLimitSensorPort);

		//Runs the collector automatically, no driver input necessary
		void Run();
		
		void ManualRoller(float power);
		
		void SetAutomaticRollerPower(float automaticpower);

		//Returns the value from the ball sensor
		int GetBallSensorValue();
		
		//Stops the collector roller and stops the arm in its current position
		void Disable();
		
		void AssistedManualRaise();
		
		void ManualRaise(int direction);
		
		void EnableProtectedMode();
		
		void DisableProtectedMode();

		//Call this function every loop, this is where all the "work" is actually done
		void Idle();
	
	private:
		Talon *LiftingTalon;
		Talon *RollerTalon;
		//AnalogChannel *BallSensor;
		DigitalInput *UpperLimitSensor;
		DigitalInput *LowerLimitSensor;
		Timer *timer;
		ArduinoI2C* I2C;
		float manualRollerTalonPower;
		enum collectorState_t{
			lowering,
			waiting,
			waitforball,
			raising,
			mellowraise,
			protectedLower,
		};
		collectorState_t collectorState;
		enum collectorMode_t{
			standard,
			manualRoller,
			disabled,
			manualRaise
		};
		collectorMode_t collectorMode;
		bool protected_mode;
		int manual_raise_direction;
		float roller_power;
};

#endif
