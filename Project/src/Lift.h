#ifndef LIFT_H
#define LIFT_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"
#include "Constants.h"

class Lift {
private:

#ifndef Practice_2017_ROBOT
    Spark liftMotor;
#else
    Victor liftMotor;
#endif

public:
	Lift();

	void raise(float speed = 1);
	void lower(); //DEPRACATED; lowering is now physically impossible
	void stop();
};

#endif
