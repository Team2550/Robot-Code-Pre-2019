#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "UDP-Receiver.h"
#include "DriveBase.h"
#include "Shooter.h"
#include "Lift.h"
#include "xBox.h"

class Robot: public IterativeRobot
{
private:
	Joystick driveController;
	Joystick perifController;

	UDP_Receiver udpReceiver;
	DriveBase driveBase;
	Shooter shooter;
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
