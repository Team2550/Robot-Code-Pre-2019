#include "Shooter.h"

Shooter::Shooter(float shooterSpeed, int motorPort) : shooterMotor(motorPort)
{
	this->shooterSpeed = shooterSpeed;
	isShooting = false;
}

void Shooter::shoot()
{
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
