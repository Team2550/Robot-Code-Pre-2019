#ifndef SHOOTER_H
#define SHOOTER_H

#include <WPILib.h>
#include "Constants.h"

class Shooter {
private:

// We don't need this construct here, but it'll make it easy to fix if the motor controller changes for one of
// the Robots.
#ifndef PRACTICE_2017_ROBOT
    VictorSP shooterMotor;
    VictorSP blenderMotor;
#else
    VictorSP shooterMotor;
    VictorSP blenderMotor;
#endif

    Timer timeSinceAdjustment;

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
