#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <WPILib.h>

class collector{

	public:
		collector(UINT8 liftingTalonPort, UINT8 rollerTalonPort, UINT8 ballSensorPort, UINT8 upperLimitSensorPort, UINT8 lowerLimitSensorPort);

		//Sets the collector state to lowering
		void Init();

		//Runs the collector automatically, no driver input necessary
		void Run();

		//Stops the collector roller and stops the arm in its current position
		void Disable();

		//Stops the collector roller and returns the arm to its upper position
		void Reset();

		//Call this function every loop, this is where all the "work" is actually done
		void Idle();
	
	private:
		Talon *LiftingTalon;
		Talon *RollerTalon;
		DigitalInput *BallSensor;
		DigitalInput *UpperLimitSensor;
		DigitalInput *LowerLimitSensor;
		Timer *BallReleaseTimer;
		enum collectorState_t{
			lowering,
			waiting,
			raising,
			releasing,
		};
		collectorState_t collectorState;
		bool enabled;
};

#endif
