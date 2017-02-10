#ifndef SHOOTER_H
#define SHOOTER_H

#include <WPILib.h>
#include "Constants.h"

class Shooter {
private:
    VictorSP shooterMotor;

public:
	Shooter();

	void shoot();
	void stop();
};

#endif
