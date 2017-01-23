#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "Shooter.h"
#include "DriveBase.h"
#include "xBox.h"

class Robot: public IterativeRobot
{
private:
	Joystick driveController;
	Joystick perifController;

	DriveBase driveBase;
	Shooter shooter;

	float shooterSpeed;
	bool xIsPressed;
	bool yIsPressed;

public:
	Robot();
	~Robot();
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
};

#endif
