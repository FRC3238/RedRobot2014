#ifndef PORTNUMS_H
#define PORTNUMS_H
#include <WPILib.h>

//Talon Ports
UINT8 leftFrontTalonPort = 3;
UINT8 leftRearTalonPort = 1;
UINT8 rightFrontTalonPort = 4;
UINT8 rightRearTalonPort = 2;
UINT8 catapultTalonOnePort = 5;
UINT8 catapultTalonTwoPort = 6;
UINT8 liftingTalonPort = 7;
UINT8 rollerTalonPort = 8;

//Digital Inputs
UINT8 catapultEncoderPortA = 1;
UINT8 catapultEncoderPortB = 2;
UINT8 ballSensorPort = 3;
UINT8 upperLimitSensorPort = 4;
UINT8 lowerLimitSensorPort = 5;
UINT8 catapultLimitSwitchPort = 6;

//Driver Station Inputs
UINT8 joystickPort = 1;

#endif
