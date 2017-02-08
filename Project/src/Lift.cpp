#include "Lift.h"

Lift::Lift() : liftMotor(Ports::Lifter::Motor)
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
