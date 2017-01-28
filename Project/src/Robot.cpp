#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
                 driveBase(0.4, 0.8, 1, 0),
				 lift()
{

}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	driveBase.RobotInit();
	lift.RobotInit();
}


void Robot::AutonomousInit()
{
	driveBase.AutoInit();
	lift.AutoInit();
}

void Robot::AutonomousPeriodic()
{
	driveBase.AutoPeriodic();
	lift.AutoPeriodic();
}

void Robot::TeleopInit()
{
	driveBase.TeleopInit();
	lift.TeleopInit();
}

void Robot::TeleopPeriodic()
{
	driveBase.TeleopPeriodic(-driveController.GetRawAxis(Controls::TankDrive::Left),
	                         -driveController.GetRawAxis(Controls::TankDrive::Right),
	                         driveController.GetRawButton(Controls::TankDrive::Boost));
	lift.TeleopPeriodic(perifController.GetRawButton(Controls::Peripherals::Climb));
}

START_ROBOT_CLASS(Robot)
