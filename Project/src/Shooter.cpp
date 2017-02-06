#include <WPILib.h>
#include "Shooter.h"

Shooter::Shooter(int shooterPort, float _shooterSpeed) :
				 shooterMotor(shooterPort), shooterSpeed(_shooterSpeed)
{
	isShooting = false;
	didDecreaseSpeed = false;
	didIncreaseSpeed = false;
}

void Shooter::shoot()
{
	shooterMotor.Set(shooterSpeed);
}

void Shooter::stop()
{
	shooterMotor.Set(0);
}
