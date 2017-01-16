#ifndef DRIVE_H
#define DRIVE_H

#include <WPILib.h>
#include "Utility.h"

class Drive {
private:
	float maxSpeed;
	VictorSP leftMotor;
	VictorSP rightMotor;
public:
	Drive(float _maxSpeed, int rightPort, int leftPort);
	void autoInit();
	void autoPeriodic();
	void teleopInit();
	void teleopPeriodic(float leftSpeed, float rightSpeed);
};

#endif
