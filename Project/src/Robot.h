#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "Shooter.h"
#include "DriveBase.h"
#include "xBox.h"

class Robot: public IterativeRobot
{
private:
	Joystick driver;
	Shooter shooter;
	DriveBase driveBase;

public:
	Robot();
	~Robot();
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
	void TestPeriodic();
};

#endif
