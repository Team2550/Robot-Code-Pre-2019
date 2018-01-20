#ifndef DRIVEBASE_H
#define DRIVEBASE_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"

class DriveBase {
private:
	Victor leftMotor;
	Victor rightMotor;

	float leftTrim;
	float rightTrim;

public:
	DriveBase(int leftMotorPort, int rightMotorPort);

	float GetLeftSpeed();
	float GetRightSpeed();
	void Drive(float leftSpeed, float rightSpeed);
	void Drive(float speed);
	void Stop();
	void SetTrim(float leftTrim, float rightTrim);
};

#endif
