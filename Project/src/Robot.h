#ifndef ROBOT_H
#define ROBOT_H

#include <AutoController.h>
#include <WPILib.h>
#include <iostream>
#include <iomanip>
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
	float autoBufferStart; // Distance at which robot will reach min speed.
	float autoBufferLength; // Length of region where the robot will slow on approach to wall

	int axisTankLeft;
	int axisTankRight;
	int buttonBoost;
	int buttonTurtle;

	Preferences *prefs;
	AutoController::InstructionSet autoDefault;
	AutoController::InstructionSet autoOther;
	frc::SendableChooser<AutoController::InstructionSet*> autoChooser;
	AutoController::InstructionSet selectedAutonomous;

	Joystick driveController;
	Joystick perifController;

	UltrasonicAnalog ultrasonic;
	LimitSwitch bumperSwitch;
	ADXRS450_Gyro gyroscope;

	DriveBase driveBase;
	AutoController autoController;

};

#endif
