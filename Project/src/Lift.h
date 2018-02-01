#ifndef LIFT_H
#define LIFT_H

#include "LimitSwitch.h"

// Controls the vertically extending lift mechanism
class Lift
{
public:
	enum MovementState { STOP, UP, DOWN };

	Lift(int topLimitSwitchPort, int bottomLimitSwitchPort,
		 float speed);

	void SetMovementSpeed(float speed);

	void MoveUp();
	void MoveDown();
	void Stop();

	MovementState GetMovementState();

	void UpdateMotors();

private:
	LimitSwitch topLimit;
	LimitSwitch bottomLimit;

	float speed;

	MovementState state;

};

#endif
