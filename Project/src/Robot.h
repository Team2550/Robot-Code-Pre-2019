#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "Drive.h"
#include "xBox.h"

class Robot: public IterativeRobot
{
private:
	Joystick driver;
	Drive drive;

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
