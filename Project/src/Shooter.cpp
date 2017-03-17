#include <WPILib.h>
#include "Shooter.h"

Shooter::Shooter() :
				 shooterMotor(Ports::Shooter::Motor),
				 blenderMotor(Ports::Shooter::BlenderMotor)

{
#ifndef PRACTICE_2017_ROBOT
	shooterMotor.SetInverted(true);
#else
#endif
	timeSinceAdjustment.Start();
	shooterSpeedOffset = 0;
}

void Shooter::shoot(float motorCurrent)
{
	if(timeSinceAdjustment.Get() >= 0.75)
			timeSinceAdjustment.Reset();

	if(motorCurrent < Speeds::Shooter::CurrentThreshold || timeSinceAdjustment.Get() >= 0.15)
		shooterMotor.Set(Speeds::Shooter::Shooter + shooterSpeedOffset);
	else
		shooterMotor.Set(Speeds::Shooter::MaxShooter);
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
