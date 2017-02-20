#ifndef SHOOTER_H
#define SHOOTER_H

#include <WPILib.h>
#include "Constants.h"

class Shooter {
private:
    VictorSP shooterMotor;
    VictorSP blenderMotor;

    float shooterSpeedOffset;
public:
	Shooter();

	void shoot(float motorCurrent = 0);
	void stop();
	void blend(bool reverse = false);
	void stopBlend();

	void setSpeedOffset(float speedOffset);
	void addSpeedOffset(float speedOffset);
	float getSpeedOffset();
};

#endif
