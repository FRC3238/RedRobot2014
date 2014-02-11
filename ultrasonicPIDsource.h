#ifndef ULTRASONICPIDSOURCE_H
#define ULTRASONICPIDSOURCE_H

#include <WPILib.h>

class ultrasonicPIDsource : public PIDSource{
	
	public:
		ultrasonicPIDsource(UINT8 leftUltrasonicPort, UINT8 rightUltrasonicPort);
		virtual double PIDGet();
		int GetUltrasonicValue();
	
	private:
		AnalogChannel *LeftUltrasonic;
		AnalogChannel *RightUltrasonic;
};

#endif
