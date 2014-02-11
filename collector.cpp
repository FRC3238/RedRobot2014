#include <WPILib.h>
#include "collector.h"

collector::collector(UINT8 liftingTalonPort, UINT8 rollerTalonPort, UINT8 ballSensorPort, UINT8 upperLimitSensorPort, UINT8 lowerLimitSensorPort){
	LiftingTalon = new Talon(liftingTalonPort);
	RollerTalon = new Talon(rollerTalonPort);
	BallSensor = new AnalogChannel(ballSensorPort);
	UpperLimitSensor = new DigitalInput(upperLimitSensorPort);
	LowerLimitSensor = new DigitalInput(lowerLimitSensorPort);
	timer = new Timer();
	collectorState = lowering;
}

void collector::ReInit(){
	collectorState = lowering;
}

void collector::Run(){
	enabled = true;
}

int collector::GetBallSensorValue(){
	return BallSensor->GetValue();
}

void collector::Disable(){
	enabled = false;
}

void collector::Idle(){
	if(enabled){
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
				if(BallSensor->GetValue() < 450){
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
					LiftingTalon->Set(0.8);
					RollerTalon->Set(1.0);
				}
				else{
					collectorState = lowering;
				}
			break;

			case mellowraise:
				if(BallSensor->GetValue() > 450){
					LiftingTalon->Set(0.4);
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
		timer->Stop();
	}
}
