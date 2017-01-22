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
	udpReceiver.autoInit();
	driveBase.autoInit();
}

void Robot::AutonomousPeriodic()
{
	udpReceiver.autoPeriodic();
	driveBase.autoPeriodic();
}

void Robot::TeleopInit()
{
	udpReceiver.teleopInit();
	driveBase.teleopInit();
}

void Robot::TeleopPeriodic()
{
	udpReceiver.teleopPeriodic();
	driveBase.teleopPeriodic(-driver.GetRawAxis(xbox::axis::leftY),
			             -driver.GetRawAxis(xbox::axis::rightY),
				    	 driver.GetRawButton(xbox::btn::rb));
}

void Robot::TestPeriodic()
{

}

START_ROBOT_CLASS(Robot)
