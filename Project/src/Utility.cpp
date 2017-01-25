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
float deadzone(float value, float tolerance)
{
	if (fabs(value) <= tolerance)
		return 0;
	return value;
}
