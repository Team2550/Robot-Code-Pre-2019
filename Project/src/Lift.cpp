#include "Lift.h"

Lifter::Lifter(int liftPort) : liftMotor(liftPort)
{

}

void Lifter::lift()
{
	liftMotor.Set(1.0);
}

void Lifter::stop()
{
	liftMotor.Set(0);
}
