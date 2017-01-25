#include "Robot.h"
#include "Utility.h"

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

}

void Robot::AutonomousInit()
{

}

void Robot::AutonomousPeriodic()
{

}

void Robot::TeleopInit()
{

}

void Robot::TeleopPeriodic()
{
	drive(deadzone(-driveController.GetRawAxis(Controls::TankDrive::Left)),
	      deadzone(-driveController.GetRawAxis(Controls::TankDrive::Right)),
	      deadzone( driveController.GetRawButton(Controls::TankDrive::Boost)));
}

START_ROBOT_CLASS(Robot)

void Robot::drive(float leftSpeed, float rightSpeed, bool boost)
{
	driveBase.drive(leftSpeed, rightSpeed, boost);
}
