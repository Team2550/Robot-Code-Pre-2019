#include "Robot.h"

// driver: (int) xBox controller number
// drive:  (float) max power, (float) max boost power, (int) left motor port,
//         (int) right motor port
Robot::Robot() : driver(0), drive(0.4, 0.8, 1, 0)
{

}

Robot::~Robot()
{

}

void Robot::RobotInit()
{

}


void Robot::AutonomousInit()
{
	drive.autoInit();
}

void Robot::AutonomousPeriodic()
{
	drive.autoPeriodic();
}

void Robot::TeleopInit()
{
	drive.teleopInit();
}

void Robot::TeleopPeriodic()
{
	drive.teleopPeriodic(-driver.GetRawAxis(xbox::axis::leftY),
			             -driver.GetRawAxis(xbox::axis::rightY),
						 driver.GetRawButton(xbox::btn::rb));
}

void Robot::TestPeriodic()
{

}

START_ROBOT_CLASS(Robot)
