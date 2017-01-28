#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
				 driveBase(driveController, perifController, 0.4, 0.8),
				 shooter(driveController, perifController, 0.82),
				 lift()
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
	shooter.AutoPeriodic();
	lift.AutoPeriodic();
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
	lift.TeleopPeriodic(perifController.GetRawButton(Controls::Peripherals::Climb));
}

START_ROBOT_CLASS(Robot)
