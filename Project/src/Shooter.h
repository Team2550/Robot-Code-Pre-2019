#ifndef SHOOTER_H
#define SHOOTER_H

#include <WPILib.h>
#include "Ports.h"

class Shooter {
private:
    VictorSP shooterMotor;

    float shooterSpeed;
public:
	Shooter();

	void shoot();
	void stop();
};

#endif
