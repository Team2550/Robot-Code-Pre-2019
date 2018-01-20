#ifndef DRIVEBASE_H
#define DRIVEBASE_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"

class DriveBase {
private:
	Victor leftMotor;
	Victor rightMotor;

	Encoder leftEncoder;
	Encoder rightEncoder;

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
	void ApplyTrim(float leftForwardsRatio, float rightForwardsRatio,
	               float leftBackwardsRatio, float rightBackwardsRatio);

	void ResetLeftDistance();
	void ResetRightDistance();
	double GetLeftDistance();
	double GetRightDistance();
};

#endif
