#include <WPILib.h>
#include "robot.h"
#include "portnums.h"

robot::robot(void){
	joystick = new Joystick(joystickPort);
	theChassis = new chassis(leftFrontTalonPort, leftRearTalonPort, rightFrontTalonPort, rightRearTalonPort);
	theCatapult = new catapult(catapultTalonOnePort, catapultTalonTwoPort, catapultEncoderPortA, catapultEncoderPortB, catapultLimitSwitchPort);
	theCollector = new collector(liftingTalonPort, rollerTalonPort, ballSensorPort, upperLimitSensorPort, lowerLimitSensorPort);
}

void robot::RobotInit() {
	SmartDashboard::init();
	theCatapult->SetMotorPower(1.0);
	insight_ballDistance.setHeader("Dis:");
	insight.registerData(insight_ballDistance, 1);
	insight.startDisplay();
}
	
void robot::DisabledInit() {
	theCollector->ReInit();
	theCatapult->ReInit();
}

void robot::AutonomousInit() {
	theCollector->Run();
}

void robot::TeleopInit() {

}
	
void robot::DisabledPeriodic() {
	theCatapult->SetStoppingPoint(0);
	int ballDistance = theCollector->GetBallSensorValue();
	insight_ballDistance.setData(ballDistance);
}

void robot::AutonomousPeriodic() {
	theCollector->Idle();
}
	
void robot::TeleopPeriodic() {
	int EncoderStoppingPoint = SmartDashboard::GetNumber("EncoderStoppingPoint");
	float x = joystick->GetRawAxis(1);
	float y = joystick->GetRawAxis(2);
	float twist = joystick->GetRawAxis(3);
	theChassis->SetJoystickData(x, y, twist);
	if(theCatapult->GetState() != 1){
		if(joystick->GetRawButton(1)){
			theCatapult->SetStoppingPoint(EncoderStoppingPoint);
			theCatapult->ResetEncoder();
			theCatapult->Fire();
		}
	}
	theCatapult->Idle();
	theChassis->Idle();
}


START_ROBOT_CLASS(robot);
