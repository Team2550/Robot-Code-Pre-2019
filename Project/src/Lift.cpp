#include "Lift.h"

Lift::Lift() : liftMotor(Ports::Lifter::Motor)
{

}

void Lift::raise(float speed)
{
	liftMotor.Set(Speeds::Lift::Lift * speed);
}

void Lift::lower(float speed)
{
	liftMotor.Set(Speeds::Lift::Lower * speed);
}

void Lift::stop()
{
	liftMotor.Set(0);
}
