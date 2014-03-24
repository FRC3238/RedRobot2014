#include <WPILib.h>
#include "robot.h"
#include "portnums.h"
#include "arduinoI2C.h"

robot::robot(void)
{
	I2C = new ArduinoI2C();
	joystick = new Joystick(joystickPort);
	theChassis = new chassis(leftFrontTalonPort, leftRearTalonPort, rightFrontTalonPort, rightRearTalonPort, leftUltrasonicPort, rightUltrasonicPort);
	theCatapult = new catapult(catapultTalonOnePort, catapultTalonTwoPort, catapultEncoderPortA, catapultEncoderPortB, catapultLimitSwitchPort);
	theCollector = new collector(liftingTalonPort, rollerTalonPort, I2C, upperLimitSensorPort, lowerLimitSensorPort);
	AutonomousTimer = new Timer();
	UnfoldingTimer = new Timer();
	AutonomousServo = new Servo(autonomousServoPort);
}

void robot::RobotInit() 
{
	theCatapult->SetMotorPower(1.0);
//	insight_ballDistance.setHeader("Diff:");
//	insight.registerData(insight_ballDistance, 1);
//	insight.startDisplay();
	SmartDashboard::init();
}
	
void robot::DisabledInit() 
{
	theCatapult->ReInit();
	AutonomousTimer->Stop();
}

void robot::AutonomousInit() 
{
	SmartDashboard::PutNumber("Potential Targets:", -666); //so if the pandaboard doesn't send data it's obvious
//	SmartDashboard::PutNumber("Target Range:", 0);
	auto_fired = false;
	AutonomousTimer->Start();
	AutonomousTimer->Reset();
	unfoldingState = collectorLower;
	//movementState = waiting;
	autonomousState = waiting;
	unfolding_done = false;
	//old_error = 999; //old_error needs a value because it is compared to the current error, but we won't have an actual old_error value until the second loop
	correct_range_loops = 0;
	error = 0;
	cummulative_error = 0;
	theCatapult->ResetEncoder();
	theCatapult->SetMotorPower(1.0);
	theCollector->SetAutomaticRollerPower(0.8);
	theCollector->Run();
}

void robot::TeleopInit() 
{
	AutonomousTimer->Stop();
	theCollector->SetAutomaticRollerPower(1.0);
}

void robot::TestInit()
{
	
}
	
void robot::DisabledPeriodic() 
{
//	float leftVoltage = theUltrasonics->GetLeftUltrasonicVoltage();
//	float rightVoltage = theUltrasonics->GetRightUltrasonicVoltage();
//	insight_ballDistance.setData(0);
//	theUltrasonics->Idle();
//	SmartDashboard::PutNumber("leftVoltage", leftVoltage);
//	SmartDashboard::PutNumber("rightVoltage", rightVoltage);
	int left = 0, right = 0, ball = 0;
	static int loops = 0;
	loops++;
	
	if(loops % 250)
	{
		I2C->GetValues(left, right,ball);
	}
	I2C->Idle();
	theCatapult->SetStoppingPoint(151);
	theCollector->DisableProtectedMode();
}

void robot::AutonomousPeriodic() {
I2C->Idle(); //updates the data acquired from I2C
	
int num_targets = static_cast<int>(SmartDashboard::GetNumber("Potential Targets:")); //gets how many targets we can see from pandaboard -666 = no data sent, 0 = no targest, 1 = cold, 2 = hot
//  float target_range = SmartDashboard::GetNumber("Target Range:");
//  float optimal_range = 120;
//  float range_tolerance = 30; //inches
  
  switch(unfoldingState){
  	  case collectorLower:
  		  if(AutonomousTimer->Get() < 1.5){ //waiting for collector to lower
  			  initial_num_targets = num_targets;
  		  }
  		  else{
  			  theCatapult->ResetLoweringTimer();
  			  unfoldingState = catapultLower;
  		  }
  	  break;
  	  
  	  case catapultLower:
  		  theCatapult->AutonomousLower();
  		  unfoldingState = waitingForCatapult;
  	  break;
  	  
  	  case waitingForCatapult:
  		  if(theCatapult->GetState() == 4){
  			  unfoldingState = done;
  		  }
  	  break;
  	  
  	  case done:
  		  if(AutonomousTimer->Get() < 3){ //giving collector time to pick up ball
  			  
  		  }
  		  else{
  			  theCollector->Disable();
  			  unfolding_done = true;
  		  }
  	  break;
  }

switch(autonomousState){

case waiting:
if(AutonomousTimer->Get() < 1){
	float x = 0;
	float y = 0;
	float twist = 0;
	theChassis->SetJoystickData(x, y, twist);
}
else{
	autonomousState = running;
}
break;

case running:
if(AutonomousTimer->Get() > 7 && !auto_fired && unfolding_done){ //Well it doesn't matter if the goal is hot, if we haven't fired yet, we need to fire
	float x = 0;
	float y = 0;
	float twist = 0;
	theChassis->SetJoystickData(x, y, twist);
	theCatapult->SetMotorPower(1.0);
	theCatapult->SetStoppingPoint(165);
	theCatapult->Fire();
	auto_fired = true;
}
else if(I2C->GetRight() > 85){ //Moving to shooting range
	error = I2C->GetRight() - 85;
	cummulative_error += error;
	float x = -.11; //correcting for chassis's tendancy to drift to one side
	float y = -(((error)*(0.006))+(cummulative_error*0.0000)); //PI control
	float twist = 0;
	theChassis->SetJoystickData(x, y, twist);
}
else if(!auto_fired && initial_num_targets > 1 && unfolding_done){ //Have we not fired and is the goal hot? If so, fire!
     float x = 0;
     float y = 0;
     float twist = 0;
     theChassis->SetJoystickData(x, y, twist);
     theCatapult->SetMotorPower(1.0);
     theCatapult->SetStoppingPoint(165);
     theCatapult->Fire();
     auto_fired = true;
}
else{
	float x = 0;
	float y = 0;
	float twist = 0;
  	theChassis->SetJoystickData(x, y, twist);
}
break;

case stopped: //For if something goes horribly wrong
	float x = 0;
	float y = 0;
 	float twist = 0;
 	theChassis->SetJoystickData(x, y, twist);
break;
}

//if(error - old_error > 12){ //If the difference between the old and current error is greater than a foot, stop robot
//	autonomousState = stopped;
//}
// Timing Only Autonomous
//  switch(movementState){
//	  case waiting:
//		  if(AutonomousTimer->Get() > 1){
//			  movementState = moving;
//		  }
//	  break;
//	  
//	  case moving:
//	  	  if(AutonomousTimer->Get() > 4.3){
//	  		  movementState = stopping;
//	  	  }
//	  	  else{
//	  		  theChassis->SetJoystickData(0, 0.6, 0);
//	  		  
//	  	  }
//	  break;
//	  
//	  case stopping:
//		  if(AutonomousTimer->Get() > 6.3){
//			  movementState = shooting;
//		  }
//		  else{
//			  theChassis->SetJoystickData(0, 0, 0);
//		  }
//	  break;
//	  
//	  case shooting:
//		  theCatapult->Fire();
//		  movementState = autonomousDone;
//	  break;
//	  
//	  case autonomousDone:
//		  
//	  break;
//  }
  
// Vision Only Autonomous
//  if (target_range - optimal_range > range_tolerance && !(auto_fired) && AutonomousTimer->Get() < 9) {
//    float x = 0;
//    float y = 0.45;
////    float y = 0.004347826*(target_range - optimal_range)+total_error*0.00000001;
//    float twist = 0;
//    theChassis->SetJoystickData(x, y, twist);
//  }
//  else if ((AutonomousTimer->Get() > 9 || 1 < num_targets) && !(auto_fir`ed)) {
//    //stop
//    float x = 0;
//    float y = 0;
//    float twist = 0;
//    theChassis->SetJoystickData(x, y, twist);
//    //shoot!
//		theCatapult->SetMotorPower(1.0);
//    	theCatapult->SetStoppingPoint(165);
//		theCatapult->Fire();
//	auto_fired = true;
//  }
//  else{
//  	float x = 0;
//  	float y = 0;
//  	float twist = 0;
//  	theChassis->SetJoystickData(x, y, twist);
//  }
	//old_error = error; //setting the old_error value for the next loop
  	theChassis->Idle();
  	theCatapult->Idle(); 
	theCollector->Idle();
}
	
void robot::TeleopPeriodic() {
	I2C->Idle();
	
	float x = joystick->GetRawAxis(1);
	float y = joystick->GetRawAxis(2);
	float twist = joystick->GetRawAxis(3);
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
		theCatapult->SetStoppingPoint(150);
	}
	if(joystick->GetRawButton(5)){
		theCatapult->SetStoppingPoint(80);
	}
	if(joystick->GetRawButton(6)){
		theCatapult->ResetEncoder();
	}
	if(joystick->GetRawButton(7)){
		theCollector->EnableProtectedMode();
	}
	if(joystick->GetRawButton(8)){
		theCollector->DisableProtectedMode();
	}
	if(joystick->GetRawButton(9)){
		theCatapult->SetStoppingPoint(100);
	}
	theCollector->ManualRaise(static_cast<int>(joystick->GetRawAxis(6)));
	theCollector->ManualRoller(static_cast<int>(joystick->GetRawAxis(5)));
	theCatapult->Idle();
	theChassis->Idle();
	theCollector->Idle();
}

void robot::TestPeriodic(){
	theCollector->AssistedManualRaise();
}

START_ROBOT_CLASS(robot);
