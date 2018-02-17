#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "xBox.h"
#include "DriveBase.h"
#include "Bulldozer.h"

class Robot: public IterativeRobot
{
public:
	Robot();
	~Robot();
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
	void DisabledInit();
	void UpdatePreferences();

private:
	float speedNormal;
	float speedTurtle;
	float speedBoost;

	int axisTankLeft;
	int axisTankRight;
	int buttonBoost;
	int buttonTurtle;
	int buttonBulldozerExtend;
	int buttonBulldozerPulse;

	bool bulldozerPulseToggle;

	Preferences *prefs;

	Joystick driveController;
	Joystick perifController;

	Timer autoTimer;

	DriveBase driveBase;
	Bulldozer bulldozer;
};

#endif
