#ifndef DRIVE_H
#define DRIVE_H
#include "WPILib.h"

class Drive {
private:

public:
	Drive();
	void autoInit();
	void autoPeriodic();
	void teleopInit();
	void teleopPeriodic();
};
