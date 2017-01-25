#ifndef SHOOTER_H
#define SHOOTER_H

#include <WPILib.h>

class Shooter {
public:
	Shooter(float shooterSpeed, int motorPort);

	void shoot();
	void shoot(float power);
	void stop();
private:
	float shooterSpeed;
	bool isShooting;

	Spark shooterMotor; // If anyone has an idea for a better name, go ahead and change it!
};

#endif
