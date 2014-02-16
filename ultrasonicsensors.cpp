#include "ultrasonicsensors.h"
#include <WPILib.h>

ultrasonicsensors::ultrasonicsensors(UINT8 leftUltrasonicPort, UINT8 rightUltrasonicPort){
	LeftUltrasonic = new AnalogChannel(leftUltrasonicPort);
	RightUltrasonic = new AnalogChannel(rightUltrasonicPort);
	leftTotal = 0;
	rightTotal = 0;
	
	DigitalModule* digMon = DigitalModule::GetInstance(1);
	this->i2c = digMon->GetI2C(38);
}

double ultrasonicsensors::PIDGet(){
	return difference;
}
void ultrasonicsensors::GetVoltages(int& left, int& right)
{
	UINT8* array = new UINT8[4];
	
	i2c->Transaction(NULL, 0, array, 4);
	
	UINT16 number1 = 0;
	UINT16 number2 = 0;
	
	//memcpy(&number1, array, 2);
	//memcpy(&number2, array, 2);
	
	SmartDashboard::PutNumber("1", number1 / 0xff);
	SmartDashboard::PutNumber("2", (UINT8)number1);
	SmartDashboard::PutNumber("3", number2 / 0xff);
	SmartDashboard::PutNumber("4", (UINT8)number2);
	
	for(int i = 0; i < 2; i++)
	{
		number1 += array[i]     * (0xFF * (i + 1));
		number2 += array[i + 2] * (0xFF * (i + 1));
	}
	
	for(int i = 0; i < 4; i++)
	{
		//char* letter = new char[1];
		//letter[0] = (char)(i + 47);
		//SmartDashboard::PutNumber(letter, (UINT8)number2);
		switch(i)
		{
		case 1:
			SmartDashboard::PutNumber("a", (UINT8)number2);
			break;
		case 2:
			SmartDashboard::PutNumber("b", (UINT8)number2);
			break;
		case 3:
			SmartDashboard::PutNumber("c", (UINT8)number2);
			break;
		case 4:
			SmartDashboard::PutNumber("d", (UINT8)number2);
			break;
		}
		
	}
		
	left = number1;
	right = number2;
}
float ultrasonicsensors::GetLeftUltrasonicVoltage()
{
	int left = 0, right = 0;
	this->GetVoltages(left, right);
	return left;
}

float ultrasonicsensors::GetRightUltrasonicVoltage(){
	int left = 0, right = 0;
	this->GetVoltages(left, right);
	return right;
}

long ultrasonicsensors::GetTotal(){
	return total;
}

void ultrasonicsensors::Idle(){
	if(leftLoopCount <= 4){
	    if(leftLoopCount == 0){
	    	leftTotal += LeftUltrasonic->GetValue();
	    }
	    else{
	    	if(abs(((leftTotal/leftLoopCount) - LeftUltrasonic->GetValue())) < 3){
	    		leftTotal += LeftUltrasonic->GetValue();
	    	}
	    	else{
	    		leftTotal += leftTotal/leftLoopCount;
	    	}
	    }
	    leftLoopCount++;
	}
	else{
		leftAverage = leftTotal/4;
		leftLoopCount = 0;
		leftTotal = 0;
	}
	if(rightLoopCount <= 4){
		if(rightLoopCount == 0){
			rightTotal += RightUltrasonic->GetValue();
		}
		else{
			if(abs(((rightTotal/rightLoopCount) - RightUltrasonic->GetValue())) < 3){
				rightTotal += RightUltrasonic->GetValue();
			}
			else{
				rightTotal += rightTotal/rightLoopCount;
			}
		}
		rightLoopCount++;
	}
	else{
		rightAverage = rightTotal/4;
		rightLoopCount = 0;
		rightTotal = 0;
	}
	total = leftAverage + rightAverage;
	difference = leftAverage - rightAverage;
}
