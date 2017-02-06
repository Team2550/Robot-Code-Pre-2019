#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
				 driveBase(Ports::TankDrive::Left, Ports::TankDrive::Right, 0.4, 0.8, 0.25),
				 shooter(Ports::Shooter::Motor, 0.82),
				 lift(driveController, perifController)
{

}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	lift.RobotInit();
}

void Robot::AutonomousInit()
{
	lift.AutoInit();
}

void Robot::AutonomousPeriodic()
{
	lift.AutoPeriodic();
}

void Robot::TeleopInit()
{
	lift.TeleopInit();
}

void Robot::TeleopPeriodic()
{
	driveBase.drive(Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Left)),
	                Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Right)),
	                driveController.GetRawButton(Controls::TankDrive::Boost),
	                driveController.GetRawButton(Controls::TankDrive::Turtle));

	if(perifController.GetRawButton(Controls::Peripherals::Shoot))
		shooter.shoot();
	else
		shooter.stop();

	lift.TeleopPeriodic();
}

START_ROBOT_CLASS(Robot)
