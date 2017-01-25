#include "Utility.h"

/*================================================
Name: deadzone
Desc: Zeroes out values within a certain "deadzone"
Arguments:
	value (I): value to deadzone
	tolerance (I): deadzone
Return:
	the input, adjusted for the deadzone
================================================*/
float Utility::deadzone(float value, float tolerance)
{
	if (fabs(value) <= tolerance)
		return 0;
	return value;
}

void Utility::setRumble(Joystick& controller, Utility::RumbleSide rumbleSide, float rumbleAmount)
{
	switch (rumbleSide)
	{
	case left:
		controller.SetRumble(Joystick::kLeftRumble, rumbleAmount);
		break;
	case right:
		controller.SetRumble(Joystick::kRightRumble, rumbleAmount);
		break;
	case both:
		controller.SetRumble(Joystick::kLeftRumble, rumbleAmount);
		controller.SetRumble(Joystick::kRightRumble, rumbleAmount);
		break;
	}
}
