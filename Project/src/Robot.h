#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include <iostream>
#include <iomanip>
#include "Utility.h"
#include "xBox.h"
#include "DriveBase.h"
#include "UltrasonicAnalog.h"
#include "LimitSwitch.h"

class Robot: public IterativeRobot
{
public:
	enum Position { LEFT, RIGHT };

	Robot();
	~Robot();
	void RobotInit();
	void RobotPeriodic();
	void AutonomousInit();
	void AutonomousPeriodic();
	void AutonomousStage(bool previousSuccess);
	void TeleopInit();
	void TeleopPeriodic();
	void DisabledInit();
	void GetGameData(Position data[3]);
	void UpdatePreferences();

private:
	float speedNormal;
	float speedTurtle;
	float speedBoost;

	Position autoStartPosition; // Position that the robot starts autonomous round at.
	float autoMinSpeed; // Speed of robot after reaching wall
	float autoMaxSpeed; // Speed of robot before reaching wall
	int autoStage;

	int axisTankLeft;
	int axisTankRight;
	int buttonBoost;
	int buttonTurtle;

	Preferences *prefs;

	Joystick driveController;
	Joystick perifController;

	UltrasonicAnalog ultrasonic;
	LimitSwitch bumperSwitch;
	Timer autoTimer;

	DriveBase driveBase;

	double callbackTime;
	Utility::ActionCallback nextTimerCallback;

	void CheckTimer();

};

#endif
