#include <WPILib.h>
#include "collector.h"

collector::collector(UINT8 liftingTalonPort, UINT8 rollerTalonPort, UINT8 ballSensorPort, UINT8 upperLimitSensorPort, UINT8 lowerLimitSensorPort){
	LiftingTalon = new Talon(liftingTalonPort);
	RollerTalon = new Talon(rollerTalonPort);
	BallSensor = new DigitalInput(ballSensorPort);
	UpperLimitSensor = new DigitalInput(upperLimitSensorPort);
	LowerLimitSensor = new DigitalInput(lowerLimitSensorPort);
	BallReleaseTimer = new Timer();
}

void collector::Init(){
	collectorState = lowering;
}

void collector::Run(){
	enabled = true;
}

void collector::Disable(){
	enabled = false;
}

void collector::Idle(){
	if(enabled){
		BallReleaseTimer->Start();
		switch(collectorState){

			case lowering:
				if(LowerLimitSensor->Get()){
					LiftingTalon->Set(-0.2);
					RollerTalon->Set(0.0);
				}
				else{
					collectorState = waiting;
				}
			break;

			case waiting:
				if(!BallSensor->Get()){
					LiftingTalon->Set(0.0);
					RollerTalon->Set(1.0);
				}
				else{
					collectorState = raising;
				}
			break;

			case raising:
				if(UpperLimitSensor->Get()){
					LiftingTalon->Set(0.4);
					RollerTalon->Set(1.0);
				}
				else{
					BallReleaseTimer->Reset();
					collectorState = releasing;
				}
			break;

			case releasing:
				if((BallReleaseTimer->Get() * 1000.0) < 500.0){
					LiftingTalon->Set(0.0);
					RollerTalon->Set(1.0);
				}
				else{
					collectorState = lowering;
				}
			break;
		
		}
	}
	else{
		LiftingTalon->Set(0.0);
		RollerTalon->Set(0.0);
		BallReleaseTimer->Stop();
	}
}
