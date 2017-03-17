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

class Robot: public IterativeRobot
{
private:
	Joystick driveController;
	Joystick perifController;
	Choreographer choreographer;

	Timer autoTimer;

	PowerDistributionPanel pdp;
	UDP_Receiver udpReceiver;
	DriveBase driveBase;
	Shooter shooter;
	Lift lift;

	Autonomous::PosScenario farLeftScenario = Autonomous::FarLeft;
	Autonomous::PosScenario middleScenario = Autonomous::Middle;
	Autonomous::PosScenario midRightScenario = Autonomous::MidRight;
	Autonomous::PosScenario farRightScenario = Autonomous::FarRight;
	Autonomous::PosScenario testScenario = Autonomous::Test;
	SendableChooser<Autonomous::PosScenario*> scenarioChooser;

	float autoSafeTime;

	bool climbToggleHold;
	bool climbToggle;
	bool decreaseShooterSpeedDown;
	bool increaseShooterSpeedDown;
	bool canAutoAim;
	bool autoSafeMode;

public:
	Robot();
	~Robot();
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
	void autoAim();
};

#endif
