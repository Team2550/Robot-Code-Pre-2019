#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include "LimitSwitch.h"

// Controls the block manipulator attached to the lift mechanism
class Manipulator
{
public:
	enum MovementState { CLOSING, CLOSED, OPENING, OPENED };

	Manipulator(int openLimitSwitchPort, int closedLimitSwitchPort,
				float speed, MovementState startingState = CLOSED);

	void SetMovementSpeed(float speed);

	void Grab();
	void Release();

	MovementState GetMovementState();

	void UpdateMotors();

private:
	LimitSwitch openLimit;
	LimitSwitch closedLimit;

	float speed;

	MovementState state;
};

#endif
