#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include <iostream>
#include <iomanip>
#include "xBox.h"
#include "AutoController.h"
#include "AutoStrategies.h"
#include "DriveBase.h"
#include "Bulldozer.h"
#include "Clamp.h"
#include "UltrasonicAnalog.h"
#include "LimitSwitch.h"
#include "Utility.h"

class Robot: public IterativeRobot
{
public:
	enum Position { LEFT, RIGHT };

	Robot();
	~Robot();
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
	void GetGameData(Position data[3]);
	void UpdatePreferences();

private:
	float speedNormal;
	float speedTurtle;
	float speedBoost;
	float boostDecelerationTime;

	int axisTankLeft;
	int axisTankRight;
	int buttonBoost;
	int buttonTurtle;
	int buttonBulldozerExtend;
	int buttonBulldozerPulse;
	int buttonBulldozerKick;
	int buttonArmToggle;
	int buttonClampToggle;

	float boostPressTime;
	bool bulldozerPulseToggle;
	bool bulldozerKickToggle;

	Preferences *prefs;
	float autoDelay;
	frc::SendableChooser<const AutoController::PositionOptions*> autoStrategyChooser;
	const AutoController::InstructionSet* selectedAutoStrategy;
	bool autoStrategyCompleted;
	bool autoStartedBackup;

	Joystick driveController;
	Joystick perifController;
	AutoController autoController;
	Timer timer;

	UltrasonicAnalog ultrasonic;
	ADXRS450_Gyro gyroscope;

	DriveBase driveBase;
	Bulldozer bulldozer;
	Clamp clamp;

};

#endif
