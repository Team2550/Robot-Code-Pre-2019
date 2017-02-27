#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "Constants.h"
#include "xBox.h"
#include "Choreographer.h"
#include "UDP-Receiver.h"
#include "DriveBase.h"
#include "Shooter.h"
#include "Lift.h"

enum DriveType
{
	Normal,
	Backwards
};

class Robot: public IterativeRobot
{
private:
	Joystick driveController;
	Joystick perifController;
	Choreographer choreographer;

	Timer blenderTimer;
	Timer autoTimer;

	PowerDistributionPanel pdp;
	UDP_Receiver udpReceiver;
	DriveBase driveBase;
	Shooter shooter;
	Lift lift;

	DriveType normalDrive;
	DriveType backwardsDrive;
	SendableChooser<DriveType*> driveChooser;

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

	void distTest();
};

#endif
