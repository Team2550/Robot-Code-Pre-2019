#include "Shooter.h"

Shooter::Shooter(int motorPort) :
                motor(motorPort)
{
	isShooting = false;
}

Shooter::~Shooter()
{

}

void Shooter::shoot(float power)
{
	if(!isShooting)
	{
		motor.Set(power);
		isShooting = true;
	}
}

void Shooter::stop()
{
	if(isShooting)
	{
		motor.Set(0);
		isShooting = false;
	}
}
