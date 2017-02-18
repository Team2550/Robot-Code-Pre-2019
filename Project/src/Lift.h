#ifndef LIFT_H
#define LIFT_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"
#include "Constants.h"

class Lift {
private:
    Spark liftMotor;

public:
	Lift();

	void lift();
	void lower(); //DEPRACATED; lowering is now physically impossible
	void stop();
};

#endif
