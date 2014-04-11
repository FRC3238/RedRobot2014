#include <WPILib.h>
#include "robot.h"
#include "portnums.h"
#include "arduinoI2C.h"

robot::robot(void) : DS(DriverStation::GetInstance()),DSEIO(DS->GetEnhancedIO())
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
//	insight_encoder_count.setHeader("Count:");
//	insight.registerData(insight_encoder_count, 1);
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
	auto_shots = 0;
	AutonomousTimer->Start();
	AutonomousTimer->Reset();
	unfoldingState = collectorLower;
	//movementState = waiting;
	autonomousState = waiting;
	unfolding_done = false;
	collector_started = false;
	//old_error = 999; //old_error needs a value because it is compared to the current error, but we won't have an actual old_error value until the second loop
	correct_range_loops = 0;
	error = 0;
	cummulative_error = 0;
	theCatapult->ResetEncoder();
	theCatapult->SetMotorPower(1.0);
	theCollector->SetAutomaticRollerPower(0.65);
	theCollector->Run();
}

void robot::TeleopInit() 
{
	AutonomousTimer->Stop();
	theCollector->SetAutomaticRollerPower(1.0);
	theCatapult->SetMotorPower(1.0);
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
	theCatapult->SetStoppingPoint(140);
	theCollector->DisableProtectedMode();
//	insight_encoder_count.setData(theCatapult->GetEncoderCount());
}

void robot::AutonomousPeriodic() {
I2C->Idle(); //updates the data acquired from I2C
	
int num_targets = static_cast<int>(SmartDashboard::GetNumber("Potential Targets:")); //gets how many targets we can see from pandaboard -666 = no data sent, 0 = no targest, 1 = cold, 2 = hot
//  float target_range = SmartDashboard::GetNumber("Target Range:");
//  float optimal_range = 120;
//  float range_tolerance = 30; //inches
 
if(DS->GetDigitalIn(2)){
switch(unfoldingState){
  	  case collectorLower:
  		  if(AutonomousTimer->Get() > .87){
  			  theCollector->Disable();
  			  theCollector->ManualRoller(0.169);
  		  }
  		  if(AutonomousTimer->Get() < 1.5){ //waiting for collector to lower
  			  initial_num_targets = num_targets;
			  theCatapult->SetMotorPower(0.2);
			  theCatapult->SetStoppingPoint(250);
			  theCatapult->Fire();
  		  }
  		  else{
  			  theCatapult->ResetLoweringTimer();
			  theCatapult->ReInit();
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
  			  unfolding_done = true;
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
if(AutonomousTimer->Get() > 6 && auto_shots == 0 && unfolding_done){ //Well it doesn't matter if the goal is hot, if we haven't fired yet, we need to fire
	float x = 0;
	float y = 0;
	float twist = 0;
	theChassis->SetJoystickData(x, y, twist);
	theCatapult->SetMotorPower(1.0);
	theCatapult->SetStoppingPoint(135); //old 140
	theCatapult->Fire();
	auto_shots++;
}
else if(AutonomousTimer->Get() > 6.25 && !collector_started){
	theCollector->Run();
	collector_started = true;
}
else if(AutonomousTimer->Get() > 9.25 && auto_shots == 1 && unfolding_done){
	theCollector->Disable();
	float x = 0;
	float y = 0;
	float twist = 0;
	theChassis->SetJoystickData(x, y, twist);
	theCatapult->SetMotorPower(1.0);
	theCatapult->SetStoppingPoint(140); //old 135
	theCatapult->Fire();
	auto_shots++;
}
else if(I2C->GetRight() > 80){ //Moving to shooting range
	error = I2C->GetRight() - 80;
	cummulative_error += error;
	float x = 0;
	float y = -(((error)*(0.006))+(cummulative_error*0.00005)); //PI control
	float twist = 0;
	theChassis->SetJoystickData(x, y, twist);
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
theChassis->Idle();
theCatapult->Idle();
theCollector->Idle();
}

else{
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
	theCatapult->SetStoppingPoint(135); //old 140
	theCatapult->Fire();
	auto_fired = true;
}
else if(I2C->GetRight() > 80){ //Moving to shooting range
	error = I2C->GetRight() - 80;
	cummulative_error += error;
	float x = 0;
	float y = -(((error)*(0.006))+(cummulative_error*0.00005)); //PI control
	float twist = 0;
	theChassis->SetJoystickData(x, y, twist);
}
//else if(!auto_fired && initial_num_targets > 1 && unfolding_done){ //Have we not fired and is the goal hot? If so, fire!
//     float x = 0;
//     float y = 0;
//     float twist = 0;
//     theChassis->SetJoystickData(x, y, twist);
//     theCatapult->SetMotorPower(1.0);
//     theCatapult->SetStoppingPoint(140); //old 135
//     theCatapult->Fire();
//     auto_fired = true;
//}
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
}
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
		theCatapult->ResetEncoder();
	}
	if(joystick->GetRawButton(5)){
		theCatapult->SetStoppingPoint(80);
	}
	if(joystick->GetRawButton(6)){
		theCatapult->SetStoppingPoint(135);
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
	theCatapult->ReInit();
	theChassis->SetJoystickData(0, 0, 0);
	theCatapult->Idle();
	theChassis->Idle();
}

START_ROBOT_CLASS(robot);
