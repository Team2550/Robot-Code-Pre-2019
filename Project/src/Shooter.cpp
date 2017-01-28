#include "Shooter.h"

Shooter::Shooter(Joystick& _driveController, Joystick& _perifController,
				 float _shooterSpeed) :
				 driveController(_driveController), perifController(_perifController),
				 shooterMotor(Ports::Shooter::Motor)
{
	shooterSpeed = _shooterSpeed;
	isShooting = false;
	didDecreaseSpeed = false;
	didIncreaseSpeed = false;
}

void Shooter::RobotInit()
{

}

void Shooter::AutoInit()
{

}

void Shooter::AutoPeriodic()
{

}

void Shooter::TeleopInit()
{

}

void Shooter::TeleopPeriodic()
{
	bool _shoot = perifController.GetRawButton(Controls::Peripherals::Shoot);
	bool increaseSpeed = perifController.GetRawButton(Controls::Peripherals::Shoot);
	bool decreaseSpeed = perifController.GetRawButton(Controls::Peripherals::Shoot);

	if(decreaseSpeed && !didDecreaseSpeed)
	{
		shooterSpeed -= 0.01;
		didDecreaseSpeed = true;
	}
	else if(!decreaseSpeed)
		didDecreaseSpeed = false;

	if(increaseSpeed && !didIncreaseSpeed)
	{
		shooterSpeed += 0.01;
		didIncreaseSpeed = true;
	}
	else if(!increaseSpeed)
		didIncreaseSpeed = false;

	frc::SmartDashboard::PutNumber("shooterSpeed", shooterSpeed);

	if(_shoot)
		shoot(shooterSpeed);
	else
		stop();
}

void Shooter::shoot(float power)
{
	if(!isShooting)
	{
		shooterMotor.Set(power);
		isShooting = true;
	}
}

void Shooter::stop()
{
	if(isShooting)
	{
		shooterMotor.Set(0);
		isShooting = false;
	}
}
