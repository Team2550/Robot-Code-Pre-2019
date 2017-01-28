#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
				 driveBase(driveController, perifController, 0.4, 0.8),
				 shooter(driveController, perifController, 0.82),
				 lift(driveController, perifController)
{

}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	driveBase.RobotInit();
	shooter.RobotInit();
	lift.RobotInit();
}


void Robot::AutonomousInit()
{
	driveBase.AutoInit();
	shooter.AutoInit();
	lift.AutoInit();
}

void Robot::AutonomousPeriodic()
{
	driveBase.AutoPeriodic();
<<<<<<< HEAD
=======
	shooter.AutoPeriodic();
	lift.AutoPeriodic();
>>>>>>> 2017-Master
}

void Robot::TeleopInit()
{
	driveBase.TeleopInit();
	shooter.TeleopInit();
	lift.TeleopInit();
}

void Robot::TeleopPeriodic()
{
	driveBase.TeleopPeriodic();
	shooter.TeleopPeriodic();
	lift.TeleopPeriodic();

	if (driveController.GetRawButton(xbox::btn::a))
	{
		Utility::setRumble(driveController, Utility::RumbleSide::both, 0.75);
	}
	else
	{
		Utility::setRumble(driveController, Utility::RumbleSide::both, 0);
	}
}

START_ROBOT_CLASS(Robot)
