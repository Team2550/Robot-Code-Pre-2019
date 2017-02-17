#ifndef SHOOTER_H
#define SHOOTER_H

#include <WPILib.h>
#include "Constants.h"

class Shooter {
private:
    VictorSP shooterMotor;
    VictorSP blenderMotor;
public:
	Shooter();

	void shoot();
	void stop();
	void blend(bool reverse = false);
	void stopBlend();
};

#endif
