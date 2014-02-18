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
		Joystick *joystick;
		chassis *theChassis;
		catapult *theCatapult;
		collector *theCollector;
		InsightLT insight;
		IntegerData insight_ballDistance;
		ArduinoI2C* I2C;
		Timer *AutonomousTimer;
		Servo *AutonomousServo;
		Timer *UnfoldingTimer;
		bool buzzer_shot;
		bool auto_fired;
//		float total_error;
		enum unfoldingState_t{
			collectorLower,
			shooterLower,
			done
		};
		unfoldingState_t unfoldingState;
};

#endif
