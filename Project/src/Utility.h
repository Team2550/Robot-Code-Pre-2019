#ifndef UTILITY_H
#define UTILITY_H

#include <WPILib.h>
#include <xBox.h>
#include <math.h>

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
Name: rumbleController
Desc: Rumbles an xbox controller
Arguments:
	controller (I): controller to rumble
Return:
	none
=================================================*/
void setRumble(Joystick& controller, Joystick::RumbleType rumbleSide, float rumbleAmount);

#endif
