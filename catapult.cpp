#include <WPILib.h>
#include "catapult.h"

catapult::catapult(UINT8 talonOnePort, UINT8 talonTwoPort, UINT8 encoderPortA, UINT8 encoderPortB, UINT8 limitSwitchPort){
	MotorOneTalon = new Talon(talonOnePort);
	MotorTwoTalon = new Talon(talonTwoPort);
	CatapultEncoder = new Encoder(encoderPortA, encoderPortB, false);
	LimitSwitch = new DigitalInput(limitSwitchPort);
	firingState = waiting;
}

void catapult::Fire(){
	firingState = firing;
}

void catapult::SetStoppingPoint(int clicks){
	stoppingClicks = clicks;
}

void catapult::SetMotorPower(float power){
	motorPower = power;
}

int catapult::GetEncoderCount(){
	return CatapultEncoder->Get();
}

void catapult::Idle(){
	switch(firingState){
		
		case waiting:
			MotorOneTalon->Set(0.0);
			MotorTwoTalon->Set(0.0);
		break;
	
	
		case firing:
			if(CatapultEncoder->Get() < stoppingClicks){
				MotorOneTalon->Set(motorPower);
				MotorTwoTalon->Set(motorPower);
			}
			else{
				MotorOneTalon->Set(0.0);
				MotorTwoTalon->Set(0.0);
				firingState = lowering;
			}
		break;
			
		case lowering:
			if(LimitSwitch->Get()){
				firingState = zeroing;
			}
			else{
				MotorOneTalon->Set(0.1);
				MotorTwoTalon->Set(0.1);
			}
		break;
	
		case zeroing:
			CatapultEncoder->Reset();
			firingState = waiting;
		break;
	}
}

