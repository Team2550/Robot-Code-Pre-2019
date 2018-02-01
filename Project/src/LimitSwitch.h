#ifndef LIMIT_SWITCH_H
#define LIMIT_SWITCH_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"

class LimitSwitch
{
public:
	LimitSwitch(int port);

	bool Get();

private:
	DigitalInput sensor;

};

#endif
