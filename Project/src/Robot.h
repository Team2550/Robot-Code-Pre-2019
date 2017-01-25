#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "UDP-Receiver.h"
#include "Shooter.h"
#include "DriveBase.h"
#include "xBox.h"
#include "Controls.h"

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
private:
	Joystick driveController;
	Joystick perifController;

	UDP_Receiver udpReceiver;
	DriveBase driveBase;
	Shooter shooter;

	bool autoAiming;

	void drive(float leftSpeed, float rightSpeed, bool boost);
	void shoot(bool shoot, bool stop, bool increaseSpeed, bool decreaseSpeed);
	void autoDrive();
};

#endif
