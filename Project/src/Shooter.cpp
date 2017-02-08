#include <WPILib.h>
#include "Shooter.h"

Shooter::Shooter() :
				 shooterMotor(Ports::Shooter::Motor), shooterSpeed(0.82)
{

}

void Shooter::shoot()
{
	shooterMotor.Set(shooterSpeed);
}

void Shooter::stop()
{
	shooterMotor.Set(0);
}
