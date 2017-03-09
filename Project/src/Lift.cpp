#include "Lift.h"

Lift::Lift() : liftMotor(Ports::Lifter::Motor)
{

}

void Lift::raise(float speed)
{
	liftMotor.Set(speed * Speeds::Lift::Lift);
}

void Lift::lower()
{
	liftMotor.Set(Speeds::Lift::Lower);
}

void Lift::stop()
{
	liftMotor.Set(0);
}

