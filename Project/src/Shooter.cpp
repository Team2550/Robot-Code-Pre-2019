#include <WPILib.h>
#include "Shooter.h"

Shooter::Shooter() :
				 shooterMotor(Ports::Shooter::Motor),
				 blenderMotor(Ports::Shooter::BlenderMotor)

{
	shooterMotor.SetInverted(true);
	shooterSpeedOffset = 0;

	blenderMotor.SetInverted(true);
}

void Shooter::shoot()
{
	shooterMotor.Set(Speeds::Shooter::Shooter + shooterSpeedOffset);
}

void Shooter::stop()
{
	shooterMotor.Set(0);
}

void Shooter::blend(bool reverse)
{
	if (reverse)
		blenderMotor.Set(-Speeds::Shooter::Blender);
	else
		blenderMotor.Set(Speeds::Shooter::Blender);
}

void Shooter::stopBlend()
{
	blenderMotor.Set(0);
}

void Shooter::setSpeedOffset(float speedOffset)
{
	float s = Speeds::Shooter::Shooter;
	shooterSpeedOffset = fmax(0, fmin(1, s + speedOffset)) - s;
}

void Shooter::addSpeedOffset(float speedOffset)
{
	float s = Speeds::Shooter::Shooter;
	shooterSpeedOffset = fmax(0, fmin(1, s + shooterSpeedOffset + speedOffset)) - s;
}

float Shooter::getSpeedOffset()
{
	return shooterSpeedOffset;
}
