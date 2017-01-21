#include "Robot.h"

Robot::Robot() : driver(0), drive(0.4, 1, 0)
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

	udpReceive();
}

void Robot::TeleopPeriodic()
{
	drive.teleopPeriodic(-driver.GetRawAxis(xbox::axis::leftY),
			             -driver.GetRawAxis(xbox::axis::rightY),
						 dataFromPi[0]);

	printf(std::to_string(dataFromPi[0]).c_str());
}

void Robot::TestPeriodic()
{

}

START_ROBOT_CLASS(Robot)
