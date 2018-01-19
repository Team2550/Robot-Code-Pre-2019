#ifndef LIMIT_SWITCH_H
#define LIMIT_SWITCH_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"

class LimitSwitch
{
private:
	DigitalInput sensor;

public:
	LimitSwitch(int port);

	bool GetPressed();
};

#endif
