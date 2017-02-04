#ifndef LIFT_H
#define LIFT_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"

class Lift {
private:
    Spark liftMotor;
public:
	Lift(int liftPort);
	void lift();
	void lower();
	void stop();
};

#endif
