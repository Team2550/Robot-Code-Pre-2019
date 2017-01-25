#include "Robot.h"
#include "Utility.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1), driveBase(0.4, 0.8, 1, 0), shooter(0.8, 2)
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
	drive(Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Left)),
	      Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Right)),
	      Utility::deadzone( driveController.GetRawButton(Controls::TankDrive::Boost)));

	shoot(perifController.GetRawButton(Controls::Peripherals::Shoot),
	      perifController.GetRawButton(Controls::Peripherals::StopShoot),
	      perifController.GetRawButton(Controls::Peripherals::IncreaseShootSpeed),
	      perifController.GetRawButton(Controls::Peripherals::DecreaseShootSpeed));

	if (driveController.GetRawButton(xbox::btn::a))
	{
		Utility::setRumble(driveController, Utility::RumbleSide::both, 0.75);
	}
	else
	{
		Utility::setRumble(driveController, Utility::RumbleSide::both, 0);
	}
}

START_ROBOT_CLASS(Robot)

void Robot::drive(float leftSpeed, float rightSpeed, bool boost)
{
	driveBase.drive(leftSpeed, rightSpeed, boost);
}

void Robot::shoot(bool shoot, bool stop, bool increaseSpeed, bool decreaseSpeed)
{
	static float shooterSpeed = 0.8;
	static bool increasedSpeed = false;
	static bool decreasedSpeed = false;

	if(decreaseSpeed && !decreasedSpeed)
	{
		shooterSpeed -= 0.01;
		decreasedSpeed = true;
	}
	else if(!decreaseSpeed)
		decreasedSpeed = false;

	if(increaseSpeed && !increasedSpeed)
	{
		shooterSpeed += 0.01;
		increasedSpeed = true;
	}
	else if(!increaseSpeed)
		increasedSpeed = false;

	frc::SmartDashboard::PutNumber("shooterSpeed", shooterSpeed);

	if(stop)
		shooter.stop();
	else if(shoot)
		shooter.shoot(shooterSpeed);
}
