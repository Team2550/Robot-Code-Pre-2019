#ifndef LIFT_H
#define LIFT_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"
#include "Constants.h"

class Lift {
private:

#ifndef PRACTICE_2017_ROBOT
    Spark liftMotor;
#else
    Talon liftMotor;
#endif

public:
	Lift();

	void raise(float speed = 1);
	[[deprecated("the Robot can no longer physically be lowered by code")]]
	void lower();
	void stop();
};

#endif
