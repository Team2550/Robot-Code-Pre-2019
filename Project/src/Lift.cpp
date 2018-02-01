#include "Lift.h"

Lift::Lift(int topLimitSwitchPort, int bottomLimitSwitchPort,
		   float speed) :
	topLimit(topLimitSwitchPort), bottomLimit(bottomLimitSwitchPort)
{
	this->speed = speed;

	state = STOP;
}

void Lift::SetMovementSpeed(float speed)
{
	this->speed = speed;
}

void Lift::MoveUp()
{
	state = UP;
}

void Lift::MoveDown()
{
	state = DOWN;
}

void Lift::Stop()
{
	state = STOP;
}

Lift::MovementState Lift::GetMovementState()
{
	return state;
}

void Lift::UpdateMotors()
{
	if (state == UP)
	{
		if (topLimit.Get())
			state == STOP;
		else
			;//TODO: Set motor to move up
	}
	else if (state == DOWN)
	{
		if (bottomLimit.Get())
			state == STOP;
		else
			;//TODO: Set motor to move down
	}

	if (state == STOP)
		;//TODO: Set motor to stop
}
