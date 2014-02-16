#include <WPILib.h>
#include "robot.h"
#include "portnums.h"
#include "ultrasonicsensors.h"

robot::robot(void){
	joystick = new Joystick(joystickPort);
	theChassis = new chassis(leftFrontTalonPort, leftRearTalonPort, rightFrontTalonPort, rightRearTalonPort, leftUltrasonicPort, rightUltrasonicPort);
	theCatapult = new catapult(catapultTalonOnePort, catapultTalonTwoPort, catapultEncoderPortA, catapultEncoderPortB, catapultLimitSwitchPort);
	theCollector = new collector(liftingTalonPort, rollerTalonPort, ballSensorPort, upperLimitSensorPort, lowerLimitSensorPort);
	AutonomousTimer = new Timer();
	UnfoldingTimer = new Timer();
	AutonomousServo = new Servo(autonomousServoPort);
}

void robot::RobotInit() {
	theCatapult->SetMotorPower(1.0);
//	insight_ballDistance.setHeader("Diff:");
//	insight.registerData(insight_ballDistance, 1);
//	insight.startDisplay();
	SmartDashboard::init();
}
	
void robot::DisabledInit() {
	theCollector->ReInit();
	theCatapult->ReInit();
	AutonomousTimer->Stop();
}

void robot::AutonomousInit() {
  SmartDashboard::PutNumber("Potential Targets:", 0);
  SmartDashboard::PutNumber("Target Range:", 0);
  auto_fired = false;
  AutonomousTimer->Start();
  AutonomousTimer->Reset();
  unfoldingState = moving;
  theCatapult->ResetEncoder();
//  theCatapult->SetStoppingPoint(5);
//  theCatapult->SetMotorPower(0.2);
  theCatapult->SetStoppingPoint(151);
  theCatapult->SetMotorPower(1.0);
}

void robot::TeleopInit() {

}
	
void robot::DisabledPeriodic() 
{
//	float leftVoltage = theUltrasonics->GetLeftUltrasonicVoltage();
//	float rightVoltage = theUltrasonics->GetRightUltrasonicVoltage();
//	insight_ballDistance.setData(0);
//	theUltrasonics->Idle();
//	SmartDashboard::PutNumber("leftVoltage", leftVoltage);
//	SmartDashboard::PutNumber("rightVoltage", rightVoltage);
	
	static ultrasonicsensors Magic(0, 0);
	static int loops = 0;
	loops++;
	int left = -1, right = -1;
	if(loops % 250)
	{
		Magic.GetVoltages(left, right);
		SmartDashboard::PutNumber("Loops", (loops/250));
		SmartDashboard::PutNumber("Left", left);
		SmartDashboard::PutNumber("Right", right);
	}
	SmartDashboard::PutNumber("BallSensor:", theCollector->GetBallSensorValue());
}

void robot::AutonomousPeriodic() {
  int   num_targets = static_cast<int>(SmartDashboard::GetNumber("Potential Targets:"));
  float target_range = SmartDashboard::GetNumber("Target Range:");
  float optimal_range = 144;
  float range_tolerance = 20; //inches
  range_tolerance = 25;
//	switch(unfoldingState){
//		case moving:
//			theCatapult->Fire();
//			unfoldingState = waiting;
//		break;
//		
//		case waiting:
//			if((AutonomousTimer->Get()*1000.0) < 3){
//				theCollector->Idle();
//			}
//			else{
//				theCatapult->ResetEncoder();
//				theCatapult->SetStoppingPoint(151);
//				theCatapult->SetMotorPower(1.0);
//				theCollector->Disable();
//				theCollector->Idle();
//				unfoldingState = done;
//			}
//		break;
//		
//		case done:
//			
//		break;
//	}
	
  if (target_range-optimal_range > range_tolerance && !(auto_fired) && AutonomousTimer->Get() < 9) {
    float x = 0;
    float y = 0.48;
    float twist = 0;
    theChassis->SetJoystickData(x, y, twist);
  }
  else if ((AutonomousTimer->Get() > 9 || 1 < num_targets) && !(auto_fired)) {
    //stop
    float x = 0;
    float y = 0;
    float twist = 0;
    theChassis->SetJoystickData(x, y, twist);
    //shoot!
		theCatapult->SetStoppingPoint(151);
		theCatapult->Fire();
	auto_fired = true;
  }
  else{
  	float x = 0;
  	float y = 0;
  	float twist = 0;
  	theChassis->SetJoystickData(x, y, twist);
  }
	theCatapult  ->Idle(); 
	theChassis   ->Idle();
	theCollector->Idle();
}
	
void robot::TeleopPeriodic() {
	float x = joystick->GetRawAxis(3);
	float y = -(joystick->GetRawAxis(2));
	float twist = joystick->GetRawAxis(1);
	theChassis->SetJoystickData(x, y, twist);
	if(joystick->GetRawButton(1)){
		theCatapult->Fire();
	}
	if(joystick->GetRawButton(2)){
		theCollector->Run();
	}
	if(joystick->GetRawButton(3)){
		theCollector->Disable();
	}
	if(joystick->GetRawButton(4)){
		theCollector->ReInit();
	}
	if(joystick->GetRawButton(5)){
		theCatapult->SetStoppingPoint(151);
	}
	if(joystick->GetRawButton(6)){
		theCatapult->ResetEncoder();
	}
	if(joystick->GetRawButton(7)){
		theCatapult->SetStoppingPoint(100);
	}
	if(joystick->GetRawButton(8)){
		theCatapult->SetStoppingPoint(80);
	}
	theCatapult->Idle();
	theChassis->Idle();
	theCollector->Idle();
}


START_ROBOT_CLASS(robot);
