#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "xBox.h"
#include "Constants.h"
#include "UDP-Receiver.h"
#include "UDP-Sender.h"
#include "DriveBase.h"
#include "Shooter.h"
#include "Lift.h"

class Robot: public IterativeRobot
{
private:
	Joystick driveController;
	Joystick perifController;
	//Timer timeSinceStart;

	Timer blenderTimer;

	UDP_Receiver udpReceiver;
	UDP_Sender udpSender;
	DriveBase driveBase;
	Shooter shooter;
	Lift lift;
	bool raiseOrLowerLift;

<<<<<<< HEAD
	Timer matchTimer;

	bool sentEndMessage;

=======
	bool decreaseShooterSpeedDown;
	bool increaseShooterSpeedDown;
>>>>>>> 2017-Drive
public:
	Robot();
	~Robot();
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
};

#endif
