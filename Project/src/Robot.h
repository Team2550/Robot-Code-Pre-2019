#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "xBox.h"
#include "Constants.h"
#include "UDP-Receiver.h"
#include "Timer.h"
#include "DriveBase.h"
#include "Shooter.h"
#include "Lift.h"

class Robot: public IterativeRobot
{
private:
	Joystick driveController;
	Joystick perifController;
	//Timer timeSinceStart;

	Timer blenderTimer;

	UDP_Receiver udpReceiver;
	DriveBase driveBase;
	Shooter shooter;
	Lift lift;

	bool climbToggleHold;
	bool climbToggle;

	bool decreaseShooterSpeedDown;
	bool increaseShooterSpeedDown;
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
