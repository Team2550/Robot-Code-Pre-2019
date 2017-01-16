#ifndef DRIVE_H
#define DRIVE_H

#include <WPILib.h>
#include "Utility.h"

class Drive {
private:
	VictorSP leftMotor;
	VictorSP rightMotor;
public:
	Drive(int rightPort, int leftPort);
	void autoInit();
	void autoPeriodic();
	void teleopInit();
	void teleopPeriodic(float speed, float rotation);
};

#endif
