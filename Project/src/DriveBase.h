#ifndef DRIVEBASE_H
#define DRIVEBASE_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"

class DriveBase
{
public:
	DriveBase(int leftMotorPort, int rightMotorPort,
			  int leftEncoderPortA, int leftEncoderPortB,
			  int rightEncoderPortA, int rightEncoderPortB,
			  double encoderDistancePerPulse);

	float GetLeftSpeed();
	float GetRightSpeed();
	void Drive(float leftSpeed, float rightSpeed);
	void Drive(float speed);
	void Stop();
	bool IsStopped();
	void ResetLeftDistance();
	void ResetRightDistance();
	double GetLeftDistance();
	double GetRightDistance();
	void SetTrim(float leftTrim, float rightTrim);

private:
	Victor leftMotor;
	Victor rightMotor;

	Encoder leftEncoder;
	Encoder rightEncoder;

	float leftTrim;
	float rightTrim;

};

#endif
