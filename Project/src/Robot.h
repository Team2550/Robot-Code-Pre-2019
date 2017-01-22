#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "UDP-Receiver.h"
#include "DriveBase.h"
#include "xBox.h"

class Robot: public IterativeRobot
{
private:
	UDP_Receiver udpReceiver;

	Joystick driver;
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
