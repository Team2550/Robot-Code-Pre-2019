#include "Utility.h"

/*================================================
Name: deadzone
Desc: Zeroes out values within a certain "deadzone"
Arguments:
	value (IO): value to deadzone
	tolerance (I): deadzone
Return:
	none
================================================*/
void Utility::deadzone(float& value, float tolerance)
{
	if (fabs(value) <= tolerance)
		value = 0;
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
