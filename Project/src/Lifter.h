#ifndef LIFTER_H
#define LIFTER_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"

class Lifter {
public:
	Lifter(int liftPort);

	void lift();
	void stop();
private:
	Spark liftMotor;
};

#endif
