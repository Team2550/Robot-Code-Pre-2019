#include <WPILib.h>
#include "Shooter.h"

Shooter::Shooter() :
				 shooterMotor(Ports::Shooter::Motor)
{
	shooterMotor.SetInverted(true);
}

void Shooter::shoot()
{
	shooterMotor.Set(Speeds::Shooter);
}

void Shooter::stop()
{
	shooterMotor.Set(0);
}
