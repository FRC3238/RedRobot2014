#ifndef ULTRASONICSENSORS_H
#define ULTRASONICSENSORS_H

#include <WPILib.h>

class ArduinoI2C 
{
	public:
		ArduinoI2C();
		void Idle();
		void GetValues(int& LeftUltrasonicSensor, int& RightUltrasonicSensor, int& BallSensor);
		int GetLeft();
		int GetRight();
		int GetBall();
		
		int Left;
		int Right;
		int Ball;
	private:
		I2C* i2c;
		
		
};

#endif
