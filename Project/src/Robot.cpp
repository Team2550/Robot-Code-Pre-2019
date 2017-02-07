#include "Robot.h"
#include "Ports.h"
#include "Controls.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
				 driveBase(),
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
	timer.Reset();
	timer.Start();
}

void Robot::AutonomousPeriodic()
{
	if(timer.Get() <= 3.000)
	{
		driveBase.driveBackwards();
	}
	else if(timer.Get() >= 8.000 && timer.Get() <= 9.000)
	{
		driveBase.driveForward();
	}
	else if(timer.Get() > 9.000 && timer.Get() <= 11.000)
	{

	}
	else
	{
		driveBase.stop();
	}
}

void Robot::TeleopInit()
{
	/* ========== DriveBase ========== */
	driveBase.stop();
}

void Robot::TeleopPeriodic()
{
	/* ========== DriveBase ========== */
	float leftSpeed = Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Left));
	float rightSpeed = Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Right));
	bool boost = driveController.GetRawButton(Controls::TankDrive::Boost);
	bool turtle = driveController.GetRawButton(Controls::TankDrive::Turtle);
	driveBase.drive(leftSpeed * (turtle ? 0.25 : (boost ? 0.8 : 0.4)),
					rightSpeed * (turtle ? 0.25 : (boost ? 0.8 : 0.4)));

	if(perifController.GetRawButton(Controls::Peripherals::Shoot))
		shooter.shoot();
	else
		shooter.stop();

	if(perifController.GetRawButton(Controls::Peripherals::Climb))
		lift.lift();
	else if(perifController.GetRawAxis(Controls::Peripherals::ClimbDown) > 0.5)
		lift.lower();
	else
		lift.stop();
}

START_ROBOT_CLASS(Robot)
