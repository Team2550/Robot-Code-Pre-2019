#ifndef DRIVEBASE_H
#define DRIVEBASE_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"

class DriveBase {
private:
	float maxSpeed;
	float maxBoostSpeed;
	VictorSP leftMotor;
	VictorSP rightMotor;
public:
	DriveBase(float _maxSpeed, float _maxBoostSpeed, int rightPort, int leftPort);
	void autoInit();
	void autoPeriodic();
	void teleopInit();
	void teleopPeriodic(float leftSpeed, float rightSpeed, bool boost);
};

#endif
