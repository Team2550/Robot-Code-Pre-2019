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
	Timer autoTimer;

	PowerDistributionPanel pdp;
	UDP_Receiver udpReceiver;
	DriveBase driveBase;
	Shooter shooter;
	Lift lift;

	bool climbToggleHold;
	bool climbToggle;

	bool decreaseShooterSpeedDown;
	bool increaseShooterSpeedDown;

	bool autoTurn = false;
	//bool bumpIndx = false;

	static const int MAX_NUM_AUTO_DRIVE_TIME = 5;
	const int airshipFrontVerticalTime = Autonomous::inchesPerSecond *  93.3;
	const int airshipBackVerticalTime = Autonomous::inchesPerSecond * 185.3;
	const int feildHorizTime = Autonomous::inchesPerSecond * 277.4;
	const int horizStretchA = feildHorizTime * .15625;

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
