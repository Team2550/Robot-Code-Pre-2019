#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include "xBox.h"
#include "AutoController.h"
#include "AutoStrategies.h"
#include "DriveBase.h"
#include "Bulldozer.h"
#include "UltrasonicAnalog.h"
#include "LimitSwitch.h"

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

	Position autoStartPosition; // Position that the robot starts autonomous round at.
	float autoMinSpeed; // Speed of robot after reaching wall
	float autoMaxSpeed; // Speed of robot before reaching wall
	float autoBufferStart; // Distance at which robot will reach min speed.
	float autoBufferLength; // Length of region where the robot will slow on approach to wall

	int axisTankLeft;
	int axisTankRight;
	int buttonBoost;
	int buttonTurtle;
	int buttonBulldozerExtend;
	int buttonBulldozerPulse;

	bool bulldozerPulseToggle;

	Preferences *prefs;
	frc::SendableChooser<const AutoController::InstructionSet*> autoStrategyChooser;
	const AutoController::InstructionSet* selectedAutoStrategy;
	bool autoStrategyCompleted;
	bool autoStartedBackup;

	Joystick driveController;
	Joystick perifController;
	AutoController autoController;
	Timer autoTimer;

	LimitSwitch bumperSwitch;
	UltrasonicAnalog ultrasonic;
	ADXRS450_Gyro gyroscope;
	BuiltInAccelerometer accelerometer;

	DriveBase driveBase;
	Bulldozer bulldozer;

};

#endif
