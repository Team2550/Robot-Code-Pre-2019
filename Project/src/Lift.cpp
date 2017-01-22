#include "Lift.h"

Lift::Lift(int liftPort) : liftMotor(liftPort)
{

}

void Lift::RobotInit()
{

}

void Lift::AutoInit()
{

}

void Lift::AutoPeriodic()
{

}

void Lift::TeleopInit()
{

}

void Lift::TeleopPeriodic(bool doLift)
{
	liftMotor.Set(doLift ? 1.0 : 0.0);
}
