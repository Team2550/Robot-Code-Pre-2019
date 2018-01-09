#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "Constants.h"
#include "xBox.h"
#include "DriveBase.h"

class Robot: public IterativeRobot
{
private:
	Joystick driveController;
	Joystick perifController;

	Timer autoTimer;

	DriveBase driveBase;

public:
	Robot();
	~Robot();
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
	void DisabledInit();
	void ClearSmartDashboard();
};

#endif
