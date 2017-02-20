#include "Utility.h"
#include <WPILib.h>

/*================================================
Name: deadzone
Desc: Zeroes out values within a certain "deadzone"
Arguments:
	value (IO): value to deadzone
	tolerance (I): deadzone
Return:
	none
================================================*/
float Utility::deadzone(float value, float tolerance)
{
	return fabs(value) <= tolerance ? 0 : value;
}

std::vector<std::string> Utility::splitString(std::string str, char delimiter)
{
	std::vector<std::string> substrings;
	std::string substr;

	for (unsigned int i = 0; i < str.length(); i++)
	{
		substr = "";

		for (; str[i] != delimiter && i < str.length(); i++)
		{
			substr += str[i];
		}

		substrings.push_back(substr);
	}

	return substrings;
}

std::vector<float> Utility::strVectorToFloatVector(std::vector<std::string> strs)
{
	std::vector<float> nums;

	for (unsigned int i = 0; i < strs.size(); i++)
		nums.push_back(stof(strs[i]));

	return nums;
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
