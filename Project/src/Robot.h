#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "DriveBase.h"
#include "UDP-Receive.c"
#include "xBox.h"

class Robot: public IterativeRobot
{
private:
	Joystick driveController;
	Joystick perifController;

	DriveBase driveBase;

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
