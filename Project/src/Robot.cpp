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

}


void Robot::AutonomousInit()
{
	driveBase.autoInit();
}

void Robot::AutonomousPeriodic()
{
	driveBase.autoPeriodic();
}

void Robot::TeleopInit()
{
	driveBase.teleopInit();

	udpReceive();
}

void Robot::TeleopPeriodic()
{
	driveBase.teleopPeriodic(-driver.GetRawAxis(xbox::axis::leftY),
			             -driver.GetRawAxis(xbox::axis::rightY),
				    	 driver.GetRawButton(xbox::btn::rb));

	printf(std::to_string(dataFromPi[0]).c_str());
}

void Robot::TestPeriodic()
{

}

START_ROBOT_CLASS(Robot)
