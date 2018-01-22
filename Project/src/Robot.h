#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include <iostream>
#include "xBox.h"
#include "DriveBase.h"
#include "CameraTracking.h"
#include "UltrasonicAnalog.h"

class Robot: public IterativeRobot
{
private:
	float speedNormal;
	float speedTurtle;
	float speedBoost;

	int axisTankLeft;
	int axisTankRight;
	int buttonBoost;
	int buttonTurtle;

	float autoCrossTime;
	float autoCrossSpeed;

	Joystick driveController;
	Joystick perifController;

	CameraTracking cameraTracking;
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
	void ClearSmartDashboard();
};

#endif
