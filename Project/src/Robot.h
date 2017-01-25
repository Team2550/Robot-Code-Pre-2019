#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "Shooter.h"
#include "DriveBase.h"
#include "xBox.h"
#include "Controls.h"

class Robot: public IterativeRobot
{
public:
	Robot();
	~Robot();
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
private:
	Joystick driveController;
	Joystick perifController;

	DriveBase driveBase;
	Shooter shooter;

	void drive(float leftSpeed, float rightSpeed, bool boost);
};

#endif
