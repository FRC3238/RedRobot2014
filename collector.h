#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <WPILib.h>
#include "arduinoI2C.h"
class collector{

	public:
		collector(UINT8 liftingTalonPort, UINT8 rollerTalonPort, ArduinoI2C* ballSensorPort, UINT8 upperLimitSensorPort, UINT8 lowerLimitSensorPort);

		//Sets the collector state to lowering
		void ReInit();

		//Runs the collector automatically, no driver input necessary
		void Run();
		
		void SetManualRollerMode();
		
		void ManualRoller(float power);

		//Returns the value from the ball sensor
		int GetBallSensorValue();
		
		//Stops the collector roller and stops the arm in its current position
		void Disable();

		//Stops the collector roller and returns the arm to its upper position
		void Reset();
		
		void ManualRaise();

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
		};
		collectorState_t collectorState;
		enum collectorMode_t{
			standard,
			manualRoller,
			disabled
		};
		collectorMode_t collectorMode;
};

#endif
