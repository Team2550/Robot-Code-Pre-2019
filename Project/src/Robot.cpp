#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1), driveBase(0.4, 0.8, 1, 0), shooter(0.82, 2)
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
	driveBase.TeleopPeriodic(-driveController.GetRawAxis(Controls::TankDrive::Left),
	                         -driveController.GetRawAxis(Controls::TankDrive::Right),
	                         driveController.GetRawButton(Controls::TankDrive::Boost));
	shooter.TeleopPeriodic(perifController.GetRawButton(Controls::Peripherals::Shoot),
	                       perifController.GetRawButton(Controls::Peripherals::StopShoot),
						   perifController.GetRawButton(Controls::Peripherals::IncreaseShootSpeed),
						   perifController.GetRawButton(Controls::Peripherals::DecreaseShootSpeed));
}

START_ROBOT_CLASS(Robot)
