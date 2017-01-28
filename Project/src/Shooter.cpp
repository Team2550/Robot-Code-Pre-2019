#include "Shooter.h"

Shooter::Shooter(float _shooterSpeed, int motorPort) : shooterMotor(motorPort)
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

void Shooter::TeleopPeriodic(bool _shoot, bool _stop, bool increaseSpeed, bool decreaseSpeed)
{
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

	if(_stop)
		stop();
	else if(_shoot)
		shoot(shooterSpeed);
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
