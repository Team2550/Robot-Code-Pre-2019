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
	//Timer timeSinceStart;

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

	bool autoTurn = false;
	//bool bumpIndx = false;

	int inchesPerSecond;
	int oneEigtheeTime;
	static const int MAX_NUM_AUTO_DRIVE_TIME = 5;
	int airshipFrontVerticalTime;
	int airshipBackVerticalTime;
	int feildHorizTime;
	int horizStretchA;

	int indx = 0;

	/*
	 * [0] = verticalStretchA
	 * [1] = ninteeTime
	 * [2] = horizStretchA
	 * [3] = ninteeTime
	 * [4] = verticalStretchB
	 */
	float autoDriveTimes[MAX_NUM_AUTO_DRIVE_TIME];

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
