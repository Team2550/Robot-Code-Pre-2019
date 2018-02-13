#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "xBox.h"
#include "DriveBase.h"

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
	int buttonlowDeckSolenoid;
	int buttonhighDeckSolenoid;
	int pneumaticTimeStamp;
	bool lowDeckSolenoidToggle = false;
	bool highDeckSolenoidToggle = false;


	Preferences *prefs;

	Joystick driveController;
	Joystick perifController;

	Solenoid lowDeckSolenoid {0};
	Solenoid highDeckSolenoid {1};

	Timer autoTimer;
	Timer pneumaticDelay;

	DriveBase driveBase;

};

#endif
