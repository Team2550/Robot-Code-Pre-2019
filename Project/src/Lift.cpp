#include "Lift.h"

Lift::Lift(int liftPort) : liftMotor(liftPort)
{

}

void Lift::lift()
{
	liftMotor.Set(1);
}

void Lift::lower()
{
	liftMotor.Set(-5);
}

void Lift::stop()
{
	liftMotor.Set(0);
}
