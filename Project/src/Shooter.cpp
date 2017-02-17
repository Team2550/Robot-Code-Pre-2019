#include <WPILib.h>
#include "Shooter.h"

Shooter::Shooter() :
				 shooterMotor(Ports::Shooter::Motor),
				 blenderMotor(Ports::Shooter::BlenderMotor)

{
	shooterMotor.SetInverted(true);
}

void Shooter::shoot()
{
	shooterMotor.Set(Speeds::Shooter::ShooterSpeed);
}

void Shooter::stop()
{
	shooterMotor.Set(0);
}

void Shooter::blend(bool reverse)
{
	if (reverse)
		blenderMotor.Set(-Speeds::Shooter::BlenderSpeed);
	else
		blenderMotor.Set(Speeds::Shooter::BlenderSpeed);
}

void Shooter::stopBlend()
{
	blenderMotor.Set(0);
}
