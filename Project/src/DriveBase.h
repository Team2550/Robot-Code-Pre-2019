#ifndef DRIVEBASE_H
#define DRIVEBASE_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"

class DriveBase {
private:
	Talon leftMotor;
	Talon rightMotor;

	bool isReversed;
public:
	DriveBase(int leftMotorPort, int rightMotorPort);

	float GetLeftSpeed();
	float GetRightSpeed();
	void Drive(float leftSpeed, float rightSpeed);
	void Drive(float speed);
	void Stop();
	void ApplyTrim(float leftForwardsRatio, float rightForwardsRatio,
	               float leftBackwardsRatio, float rightBackwardsRatio);

	void SetReversed(bool reverse);
	bool GetReversed();
};

#endif
