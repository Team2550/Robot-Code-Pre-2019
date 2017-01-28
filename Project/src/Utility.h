#ifndef UTILITY_H
#define UTILITY_H

#include <WPILib.h>
#include <xBox.h>
#include <math.h>

namespace Utility
{

	enum RumbleSide {left, right, both};

	/*=================================================
	Name: deadzone
	Desc: Zeroes out values within a certain "deadzone"
	Arguments:
		value (IO): value to deadzone
		tolerance (I): deadzone
	Return:
		none
	=================================================*/
	void deadzone(float& value, float tolerance = 0.2);

	/*=================================================
	Name: setRumble
	Desc: Rumbles an xbox controller
	Arguments:
		controller   (I): controller to rumble
		rumbleSide   (I): side to rumble
		rumbleAmount (I): rumble force [0, 1]
	Return:
		none
	=================================================*/
	void setRumble(Joystick& controller, RumbleSide rumbleSide, float rumbleAmount);

}

#endif
