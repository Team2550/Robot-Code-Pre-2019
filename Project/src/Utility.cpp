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
void deadzone(float& value, float tolerance)
{
	if (fabs(value) <= tolerance)
		value = 0;
}
