#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
				 driveBase(Ports::TankDrive::Left, Ports::TankDrive::Right, 0.4, 0.8, 0.25),
				 shooter(driveController, perifController, 0.82),
				 lift(driveController, perifController)
{

}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	shooter.RobotInit();
	lift.RobotInit();
}


void Robot::AutonomousInit()
{
	shooter.AutoInit();
	lift.AutoInit();
}

void Robot::AutonomousPeriodic()
{
	shooter.AutoPeriodic();
	lift.AutoPeriodic();
}

void Robot::TeleopInit()
{
	shooter.TeleopInit();
	lift.TeleopInit();
}

void Robot::TeleopPeriodic()
{
	driveBase.drive(Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Left)),
	                Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Right)),
	                driveController.GetRawButton(Controls::TankDrive::Boost),
	                driveController.GetRawButton(Controls::TankDrive::Turtle));
	shooter.TeleopPeriodic();
	lift.TeleopPeriodic();
}

START_ROBOT_CLASS(Robot)
