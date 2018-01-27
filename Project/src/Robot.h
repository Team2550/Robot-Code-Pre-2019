#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include <iostream>
#include "xBox.h"
#include "DriveBase.h"
#include "UltrasonicAnalog.h"

class Robot: public IterativeRobot
{
private:
	float speedNormal;
	float speedTurtle;
	float speedBoost;

	float autoBufferStart;
	float autoMinSpeed;
	float autoBufferLength;

	int axisTankLeft;
	int axisTankRight;
	int buttonBoost;
	int buttonTurtle;

	Preferences *prefs;

	Joystick driveController;
	Joystick perifController;

	UltrasonicAnalog ultrasonic;
	Timer autoTimer;

	DriveBase driveBase;

public:
	Robot();
	~Robot();
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
	void DisabledInit();
	void GetGameData(bool data[3]);
	void UpdatePreferences();
};

#endif
