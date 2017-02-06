#ifndef SHOOTER_H
#define SHOOTER_H

#include <WPILib.h>

class Shooter {
private:
    VictorSP shooterMotor; // If anyone has an idea for a better name, go ahead and change it!
	bool isShooting;

	float shooterSpeed;
	bool didDecreaseSpeed;
	bool didIncreaseSpeed;
public:
	Shooter(int shooterPort, float _shooterSpeed);

	void shoot();
	void stop();
};

#endif
