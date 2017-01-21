#ifndef SHOOTER_H
#define SHOOTER_H

#include <wpilib.h>

class Shooter
{
public:
	Shooter(int motorPort);
	~Shooter();

	void shoot(float power = 1);
	void stop();
private:
	Spark motor; // If anyone has an idea for a better name, go ahead and change it!
	bool isShooting;
};

#endif
