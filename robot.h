#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "chassis.h"
#include "catapult.h"
#include "collector.h"
#include "arduinoI2C.h"
#include "InsightLT_CPP/InsightLT.h"
using namespace insight;

class robot : public IterativeRobot {
	
	public:
		robot(void);
		void RobotInit(void);
		void DisabledInit(void);
		void DisabledPeriodic(void);
		void AutonomousInit(void);
		void AutonomousPeriodic(void);
		void AutonomousContinuous(void);
		void TeleopInit(void);
		void TeleopPeriodic(void);
		void TestInit(void);
		void TestPeriodic(void);
	
	private:
		DriverStation *DS;
		DriverStationEnhancedIO &DSEIO;
		Joystick *joystick;
		chassis *theChassis;
		catapult *theCatapult;
		collector *theCollector;
//		InsightLT insight;
//		IntegerData insight_encoder_count;
		ArduinoI2C* I2C;
		Timer *AutonomousTimer;
		Servo *AutonomousServo;
		Timer *UnfoldingTimer;
		bool buzzer_shot;
		bool auto_fired;
		int auto_shots;
		bool unfolding_done;
		bool collector_started;
		int correct_range_loops;
		int initial_num_targets;
//		float total_error;
		int error;
		//int old_error;
		int cummulative_error;
		enum unfoldingState_t{
			collectorLower,
			catapultLower,
			waitingForCatapult,
			done
		};
		unfoldingState_t unfoldingState;
		enum autonomousState_t{
			waiting,
			running,
			stopped
		};
		autonomousState_t autonomousState;
//		For timing only autonomous
//		enum movementState_t{
//			waiting,
//			moving,
//			stopping,
//			shooting,
//			autonomousDone
//		};
//		movementState_t movementState;
};

#endif
