/*
 * Robot.h
 *
 *  Created on: Nov 13, 2018
 *      Author: Brayton Kerekffy
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <WPILib.h>
#include <iostream>
#include <iomanip>

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
	//void GetGameData(Position data[3]);
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

	float boostPressTime;

	Preferences *prefs;
	float autoDelay;
	//frc::SendableChooser<const AutoController::PositionOptions*> autoStrategyChooser;
	//const AutoController::InstructionSet* selectedAutoStrategy;
	bool autoStrategyCompleted;
	bool autoStartedBackup;

	Joystick driveController;
	Joystick perifController;
	//AutoController autoController;
	Timer timer;

	//UltrasonicAnalog ultrasonic;
	ADXRS450_Gyro gyroscope;

	//DriveBase driveBase;
};

#endif /* ROBOT_H_ */
