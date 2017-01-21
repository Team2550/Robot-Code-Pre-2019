#ifndef LIFT_H
#define LIFT_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"

class Lift {
private:

public:
	Lift();
	void autoInit();
	void autoPeriodic();
	void teleopInit();
	void teleopPeriodic();
};

#endif
