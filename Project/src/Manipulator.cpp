#include "Manipulator.h"

Manipulator::Manipulator(int openLimitSwitchPort, int closedLimitSwitchPort,
						 float speed, MovementState startingState = CLOSED) :
	openLimit(openLimitSwitchPort), closedLimit(closedLimitSwitchPort)
{
	this->speed = speed;

	state = startingState;
}

void Manipulator::SetMovementSpeed(float speed)
{
	this->speed = speed;
}

void Manipulator::Grab()
{
	if (state != CLOSED)
		state = CLOSING;
}

void Manipulator::Release()
{
	if (state != OPENED)
		state = OPENING;
}

Manipulator::MovementState Manipulator::GetMovementState()
{
	return state;
}

void Manipulator::UpdateMotors()
{
	if (state == OPENING)
	{
		if (openLimit.Get())
			state == OPENED;
		else
			;//TODO: Set motor speed to open manipulator
	}
	else if (state == CLOSING)
	{
		if (closedLimit.Get())
			state == CLOSED;
		else
			;//TODO: Set motor speed to close manipulator
	}

	if (state == OPENED || state == CLOSED)
		;//TODO: Set motor speed to 0
}
