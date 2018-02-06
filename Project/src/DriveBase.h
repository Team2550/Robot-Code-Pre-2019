#ifndef DRIVEBASE_H
#define DRIVEBASE_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"

class DriveBase
{
public:
	DriveBase(int leftMotorPort, int rightMotorPort);

	double GetLeftSpeed();
	double GetRightSpeed();
	void Drive(double leftSpeed, double rightSpeed);
	void Drive(double speed);
	void Stop();
	void SetTrim(float leftTrim, float rightTrim);

private:
	Victor leftMotor;
	Victor rightMotor;

	float leftTrim;
	float rightTrim;

};

#endif
