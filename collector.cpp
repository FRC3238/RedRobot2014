#include <WPILib.h>
#include "collector.h"

collector::collector(UINT8 liftingTalonPort, UINT8 rollerTalonPort, ArduinoI2C* I2C, UINT8 upperLimitSensorPort, UINT8 lowerLimitSensorPort){
	LiftingTalon = new Talon(liftingTalonPort);
	RollerTalon = new Talon(rollerTalonPort);
	//BallSensor = new AnalogChannel(ballSensorPort);
	UpperLimitSensor = new DigitalInput(upperLimitSensorPort);
	LowerLimitSensor = new DigitalInput(lowerLimitSensorPort);
	timer = new Timer();
	collectorState = lowering;
	this->I2C = I2C;
}

void collector::ReInit(){
	collectorState = lowering;
}

void collector::Run(){
	collectorMode = standard;
}

void collector::SetManualRollerMode(){
	collectorMode = manualRoller;
}

void collector::ManualRoller(float power){
	manualRollerTalonPower = power;
}

int collector::GetBallSensorValue()
{
	SmartDashboard::PutNumber("Ball Value", I2C->Ball);
	return  I2C->Ball;
}

void collector::Disable(){
	collectorMode = disabled;
}

void collector::AssistedManualRaise(){
	LiftingTalon->Set(0.3);
}

void collector::ManualRaise(){
	collectorMode = manualRaise;
}

void collector::Idle(){
	switch(collectorMode){
	
	case standard:
		timer->Start();
		switch(collectorState){

			case lowering:
				if(LowerLimitSensor->Get()){
					LiftingTalon->Set(-0.4);
					RollerTalon->Set(0.0);
				}
				else{
					collectorState = waiting;
				}
			break;

			case waiting:
				if(GetBallSensorValue() < 350){
					LiftingTalon->Set(0.0);
					RollerTalon->Set(1.0);
				}
				else{
					timer->Reset();
					collectorState = waitforball;
				}
			break;
			
			case waitforball:
				if((timer->Get() * 1000.0) < 500.0){
					LiftingTalon->Set(0.0);
					RollerTalon->Set(1.0);
				}
				else{
					timer->Reset();
					collectorState = raising;
				}
			break;
				
			case raising:
				if((timer->Get() * 1000.0) < 750.0){
					LiftingTalon->Set(0.6);
					RollerTalon->Set(1.0);
				}
				else{
					collectorState = lowering;
				}
			break;

			case mellowraise:
				if(GetBallSensorValue() > 350){
					LiftingTalon->Set(0.3);
					RollerTalon->Set(1.0);
				}
				else{
					collectorState = lowering;
				}
			break;
		}
	break;
	
	case disabled:
		LiftingTalon->Set(0.0);
		RollerTalon->Set(0.0);
		timer->Stop();
	break;
	
	case manualRoller:
		RollerTalon->Set(manualRollerTalonPower);
		LiftingTalon->Set(0.0);
		timer->Stop();
	break;
	
	case manualRaise:
		LiftingTalon->Set(0.45);
	break;
	}
}
