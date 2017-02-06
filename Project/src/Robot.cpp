#include "Robot.h"
#include "Ports.h"
#include "Controls.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
				 driveBase(Ports::TankDrive::Left, Ports::TankDrive::Right, 0.4, 0.8, 0.25),
				 shooter(Ports::Shooter::Motor, 0.82),
				 lift(Ports::Lifter::Motor)
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
	driveBase.drive(Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Left)),
	                Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Right)),
	                driveController.GetRawButton(Controls::TankDrive::Boost),
	                driveController.GetRawButton(Controls::TankDrive::Turtle));

	if(perifController.GetRawButton(Controls::Peripherals::Shoot))
		shooter.shoot();
	else
		shooter.stop();

	if(perifController.GetRawButton(xbox::btn::lb))
		lift.lift();
	else if(perifController.GetRawAxis(Controls::Peripherals::ClimbDown) > 0.5)
		lift.lower();
	else
		lift.stop();
}

START_ROBOT_CLASS(Robot)
