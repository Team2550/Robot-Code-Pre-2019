#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "DriveBase.h"
#include "Lift.h"
#include "xBox.h"
#include "Controls.h"

class Robot: public IterativeRobot
{
private:
	Joystick driveController;
	Joystick perifController;

	DriveBase driveBase;
	Lift lift;

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
