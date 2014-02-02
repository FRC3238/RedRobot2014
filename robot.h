#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
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
	
	private:
		
};

#endif
