#ifndef SHOOTER_H
#define SHOOTER_H

#include <WPILib.h>

class Shooter {
private:
	Spark shooterMotor; // If anyone has an idea for a better name, go ahead and change it!
	bool isShooting;

	float shooterSpeed;
	bool didDecreaseSpeed;
	bool didIncreaseSpeed;
public:
	Shooter(float _shooterSpeed, int motorPort);
	void RobotInit();
	void AutoInit();
	void AutoPeriodic();
	void TeleopInit();
	void TeleopPeriodic(bool _shoot, bool increaseSpeed, bool decreaseSpeed);

	void shoot(float power = 1);
	void stop();
};

#endif
