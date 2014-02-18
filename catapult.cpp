#include <WPILib.h>
#include "catapult.h"

catapult::catapult(UINT8 talonOnePort, UINT8 talonTwoPort, UINT8 encoderPortA, UINT8 encoderPortB, UINT8 limitSwitchPort){
	MotorOneTalon = new Talon(talonOnePort);
	MotorTwoTalon = new Talon(talonTwoPort);
	CatapultEncoder = new Encoder(encoderPortA, encoderPortB, true);
	LoweringTimer = new Timer();
	LoweringTimer->Start();
	CatapultEncoder->Start();
	firingState = waiting;
}

void catapult::ReInit(){
	firingState = waiting;
}

void catapult::Fire(){
	firingState = firing;
}

void catapult::AutonomousLower(){
	firingState = autonomousLowering;
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

int catapult::GetState(){
	return firingState;
}

void catapult::ResetEncoder(){
	CatapultEncoder->Reset();
}

void catapult::ResetLoweringTimer(){
	LoweringTimer->Reset();
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
				LoweringTimer->Reset();
				firingState = lowering;
			}
		break;
			
		case lowering:
			if((LoweringTimer->Get() * 1000.0) < 1250.0){
				MotorOneTalon->Set(-0.07);
				MotorTwoTalon->Set(-0.07);
			}
			else{
				firingState = zeroing;
			}
		break;
	
		case zeroing:
			CatapultEncoder->Reset();
			firingState = waiting;
		break;
		
		case autonomousLowering:
			if((LoweringTimer->Get() * 1000.0) < 500.0){
				MotorOneTalon->Set(-0.3);
				MotorTwoTalon->Set(-0.3);
			}
			else{
				firingState = zeroing;
			}
		break;
	}
}

