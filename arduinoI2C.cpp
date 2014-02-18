#include "ArduinoI2C.h"
#include <WPILib.h>

ArduinoI2C::ArduinoI2C()
{

	DigitalModule* digMon = DigitalModule::GetInstance(1);
	i2c = digMon->GetI2C(38);
}

void ArduinoI2C::Idle()
{
	static int loops = 0;
	loops++;
	if(loops % 25)
	{
		UINT8* array = new UINT8[6];
		UINT16 number[] = {0, 0, 0};
		i2c->Transaction(NULL, 0, array, 6);
		static DriverStationLCD* LCD = DriverStationLCD::GetInstance();
		
		
		for(int i = 0; i < 3; i++)
		{
			memcpy(number + i, array + (2 * i), 2);
		}
		//for(int i = 0; i < 3; i++)
		//{
	//			LCD->PrintfLine(DriverStationLCD::Line(i), "%i: %i   \t %i, :%i", 
	//					i, array[i], (i%2)?(number[i/2] / 0xff):((UINT8)number[i/2]), number[i/2]);
		//}
		
		
		this->Left  = number[0];
		this->Right = number[1];
		this->Ball  = number[2];
		
		LCD->PrintfLine(DriverStationLCD::Line(0), "Left:  %i", Left);
		LCD->PrintfLine(DriverStationLCD::Line(1), "Right: %i", Right);
		LCD->PrintfLine(DriverStationLCD::Line(2), "Ball:  %i", Ball);
		LCD->PrintfLine(DriverStationLCD::Line(3), "Loops: %i", (int)loops);
		LCD->UpdateLCD();
	}
}
void ArduinoI2C::GetValues(int& left, int& right, int& ball)
{
	left = Left;
	right = Right;
	ball = Ball;
}

int ArduinoI2C::GetLeft()
{
	return Left;
}
int ArduinoI2C::GetRight()
{
	return Right;
}
int ArduinoI2C::GetBall()
{
	return Ball;
}

/*
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
	UINT8* array = new UINT8[6];
		
		i2c->Transaction(NULL, 0, array, 6);
		//static DriverStationLCD* LCD = DriverStationLCD::GetInstance();
		
		UINT16 number[] = {0, 0, 0};
		for(int i = 0; i < 3; i++)
		{
			memcpy(number + i, array + (2 * i), 2);
		}
		for(int i = 0; i < 6; i++)
		{
//			LCD->PrintfLine(DriverStationLCD::Line(i), "%i: %i   \t %i, :%i", 
//					i, array[i], (i%2)?(number[i/2] / 0xff):((UINT8)number[i/2]), number[i/2]);
			
		}
		
		
		//SmartDashboard::PutNumber("BallSensor v2", number3);
		//LCD->UpdateLCD();
		left = number[0];
		right = number[1];
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
*/
