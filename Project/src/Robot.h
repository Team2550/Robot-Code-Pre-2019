#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "Constants.h"
#include "xBox.h"
#include "UDP-Receiver.h"
#include "DriveBase.h"
#include "Shooter.h"
#include "Lift.h"

class Robot: public IterativeRobot
{
private:
	Joystick driveController;
	Joystick perifController;

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

	Autonomous::Scenario middleScenario = Autonomous::Middle;
	Autonomous::Scenario sideLeftScenario = Autonomous::SideLeft;
	Autonomous::Scenario sideRightScenario = Autonomous::SideRight;
	SendableChooser<Autonomous::Scenario*> autoScenarioChooser;

	bool targetRumbleOff = false;
	bool targetRumbleOn = true;
	SendableChooser<bool*> targetRumbleChooser;

	Autonomous::Ready *autoReady;
	Autonomous::Scenario *autoScenario;
	bool *targetRumble;

	bool wasAtTarget;
	float reachedTargetTime;
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
	void DisabledInit();
	bool autoAim(bool side, float startCurrentSensingTime = 0);
	void clearSmartDashboard();
};

#endif
