#ifndef DRIVE_H
#define DRIVE_H

#include <WPILib.h>
#include "Utility.h"

class Drive {
private:
	float maxSpeed;
	float maxBoostSpeed;
	VictorSP leftMotor;
	VictorSP rightMotor;
public:
	Drive(float _maxSpeed, float _maxBoostSpeed, int rightPort, int leftPort);
	void autoInit();
	void autoPeriodic();
	void teleopInit();
	void teleopPeriodic(float leftSpeed, float rightSpeed, bool boost);
};

#endif
