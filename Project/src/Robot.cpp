#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driver(0), driveBase(0.4, 0.8, 1, 0)
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
	driveBase.TeleopPeriodic(-driver.GetRawAxis(xbox::axis::leftY),
			                 -driver.GetRawAxis(xbox::axis::rightY),
					    	 driver.GetRawButton(xbox::btn::rb));
}

START_ROBOT_CLASS(Robot)
