#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
				 driveBase(driveController, perifController, 0.4, 0.8, 0.25),
				 shooter(driveController, perifController, 0.82),
				 lift(Ports::Lifter::Motor)
{

}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	driveBase.RobotInit();
	shooter.RobotInit();
}


void Robot::AutonomousInit()
{
	driveBase.AutoInit();
	shooter.AutoInit();
}

void Robot::AutonomousPeriodic()
{
	driveBase.AutoPeriodic();
	shooter.AutoPeriodic();
}

void Robot::TeleopInit()
{
	driveBase.TeleopInit();
	shooter.TeleopInit();
}

void Robot::TeleopPeriodic()
{
	driveBase.TeleopPeriodic();
	shooter.TeleopPeriodic();
	if(perifController.GetRawButton(Controls::Peripherals::Climb))
		lift.lift();
	else if(perifController.GetRawButton(Controls::Peripherals::ClimbDown))
		lift.lower();
	else
		lift.stop();
}

START_ROBOT_CLASS(Robot)
