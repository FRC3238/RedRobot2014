#ifndef ULTRASONICSENSORS_H
#define ULTRASONICSENSORS_H

#include <WPILib.h>

class ultrasonicsensors : public PIDSource{
	
	public:
		ultrasonicsensors(UINT8 leftUltrasonicPort, UINT8 rightUltrasonicPort);
		virtual double PIDGet();
		void GetVoltages(int& left, int& right);
		float GetLeftUltrasonicVoltage();
		float GetRightUltrasonicVoltage();
		long GetTotal();
		void Idle();
	
	private:
		I2C* i2c;
		int leftLoopCount;
		int rightLoopCount;
		long leftTotal;
		long rightTotal;
		long leftAverage;
		long rightAverage;
		long difference;
		long total;
		AnalogChannel *LeftUltrasonic;
		AnalogChannel *RightUltrasonic;
};

#endif
