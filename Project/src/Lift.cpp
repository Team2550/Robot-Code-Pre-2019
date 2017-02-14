#include "Lift.h"

Lift::Lift() : liftMotor(Ports::Lifter::Motor)
{

}

void Lift::raise()
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
