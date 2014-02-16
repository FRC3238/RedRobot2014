#ifndef PORTNUMS_H
#define PORTNUMS_H
#include <WPILib.h>

//Talon Ports
UINT8 leftFrontTalonPort = 6;
UINT8 leftRearTalonPort = 8;
UINT8 rightFrontTalonPort = 7;
UINT8 rightRearTalonPort = 3;
UINT8 catapultTalonOnePort = 4;
UINT8 catapultTalonTwoPort = 5;
UINT8 liftingTalonPort = 1;
UINT8 rollerTalonPort = 2;

//Servo Ports
UINT8 autonomousServoPort = 9;

//Digital Inputs
UINT8 catapultEncoderPortA = 5;
UINT8 catapultEncoderPortB = 6;
UINT8 upperLimitSensorPort = 4;
UINT8 lowerLimitSensorPort = 3;
UINT8 catapultLimitSwitchPort = 3;

//Analog Inputs
UINT8 ballSensorPort = 6;
UINT8 leftUltrasonicPort = 4;
UINT8 rightUltrasonicPort = 5;

//Driver Station Inputs
UINT8 joystickPort = 1;

#endif
