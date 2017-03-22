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

	Autonomous::Ready safeReady = Autonomous::Safe;
	Autonomous::Ready blindReady = Autonomous::Blind;
	Autonomous::Ready visionReady = Autonomous::Vision;
	SendableChooser<Autonomous::Ready*> autoReadyChooser;

	Autonomous::PosScenario middleScenario = Autonomous::Middle;
	Autonomous::PosScenario sideScenario = Autonomous::Side;
	SendableChooser<Autonomous::PosScenario*> autoScenarioChooser;

	Autonomous::Ready *autoReady;

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
	void autoAim();
	void clearSmartDashboard();
};

#endif
