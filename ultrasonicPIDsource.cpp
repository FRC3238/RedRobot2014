#include "ultrasonicPIDsource.h"
#include <WPILib.h>

ultrasonicPIDsource::ultrasonicPIDsource(UINT8 leftUltrasonicPort, UINT8 rightUltrasonicPort){
	LeftUltrasonic = new AnalogChannel(leftUltrasonicPort);
	RightUltrasonic = new AnalogChannel(rightUltrasonicPort);
	LeftUltrasonic->SetAverageBits(8);
	RightUltrasonic->SetAverageBits(8);
}

double ultrasonicPIDsource::PIDGet(){
	double difference = (LeftUltrasonic->GetAverageValue())-(RightUltrasonic->GetAverageValue());
	return difference;
}

int ultrasonicPIDsource::GetUltrasonicValue(){
	return LeftUltrasonic->GetValue();
}
