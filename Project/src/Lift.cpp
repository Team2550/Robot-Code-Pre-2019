#include "Lift.h"

Lift::Lift(int liftPort) : liftMotor(liftPort)
{

}

void Lift::autoInit()
{

}

void Lift::autoPeriodic()
{

}

void Lift::teleopInit()
{

}

void Lift::teleopPeriodic(bool doLift)
{
	liftMotor.Set(doLift ? 1.0 : 0.0);
}
