#ifndef SHOOTER_H
#define SHOOTER_H

#include <WPILib.h>
#include "Ports.h"
#include "Controls.h"

class Shooter {
private:
    Joystick& driveController;
    Joystick& perifController;

	Spark shooterMotor; // If anyone has an idea for a better name, go ahead and change it!
	bool isShooting;

	float shooterSpeed;
	bool didDecreaseSpeed;
	bool didIncreaseSpeed;
public:
	Shooter(Joystick& _driveController, Joystick& _perifController,
			float _shooterSpeed);
	void RobotInit();
	void AutoInit();
	void AutoPeriodic();
	void TeleopInit();
	void TeleopPeriodic();

	void shoot(float power = 1);
	void stop();
};

#endif
