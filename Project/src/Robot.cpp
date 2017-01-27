#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1), driveBase(0.4, 0.8, 1, 0)
{

}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	driveBase.RobotInit();
}


void Robot::AutonomousInit()
{
	driveBase.AutoInit();
}

void Robot::AutonomousPeriodic()
{
	driveBase.AutoPeriodic();
}

void Robot::TeleopInit()
{
	driveBase.TeleopInit();
}

void Robot::TeleopPeriodic()
{
	driveBase.TeleopPeriodic(-driveController.GetRawAxis(xbox::axis::leftY),
			                 -driveController.GetRawAxis(xbox::axis::rightY),
							 driveController.GetRawButton(xbox::btn::rb));

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
