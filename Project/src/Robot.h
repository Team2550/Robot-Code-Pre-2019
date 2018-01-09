#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "xBox.h"
#include "DriveBase.h"
#include "grip/GripPipeline.h"

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

	Joystick driveController;
	Joystick perifController;

	cs::CvSink cvSink; // OpenCV image data sink
	Timer autoTimer;

	DriveBase driveBase;
	grip::GripPipeline gripPipeline;

public:
	Robot();
	~Robot();
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
	void DisabledInit();
	void ClearSmartDashboard();
};

#endif
