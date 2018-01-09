#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "xBox.h"

namespace Ports
{
#ifndef NORMALIZED_CODE_BASED_ON_2017_CODE
	namespace TankDrive
	{
		const int Left = 1;
		const int Right = 0;
	}


	namespace PDP
	{
		const int LeftMotor1 = 12;
		const int LeftMotor2 = 13;
		const int RightMotor1 = 14;
		const int RightMotor2 = 15;
	}
#else
	namespace TankDrive
	{
		const int Left = 1;
		const int Right = 0;
	}

	namespace PDP
	{

		const int LeftMotor1 = 12;
		const int LeftMotor2 = 13;
		const int RightMotor1 = 14;
		const int RightMotor2 = 15;
	}
#endif
}

namespace Controls
{
	namespace TankDrive
	{
		const int Left = xbox::axis::leftY;
		const int Right = xbox::axis::rightY;
		const int Boost = xbox::btn::rb;
		const int Turtle = xbox::btn::lb;
		const int AutoAim = xbox::btn::a;
	}
}

namespace Speeds
{
	namespace DriveBase
	{
		const float Normal = 0.5;
		const float Boost = 1;
		const float Turtle = 0.25;
		const float LeftPowerRatioBackwards = 1;
		const float RightPowerRatioBackwards = 1;
		const float LeftPowerRatioForwards = 1;
		const float RightPowerRatioForwards = 1;
	}

}

namespace Autonomous
{
	const float AmpLimit = 2;

	enum Ready // What auto functions are ready
	{
		Safe, // Safe mode (pass baseline) is ready
		Blind, // Putting gear on peg blindly is ready
		Vision // Putting gear on peg with vision is ready
	};

	enum Scenario
	{
		Scenario1,
		Scenario2,
		Scenario3
	};

	const Scenario DefaultScenario = Scenario1;

}

namespace UDP
{
	const int MaxPacketsFlush = 20;

	const int DataCount = 6;

	namespace Index
	{
		const int PercentMatch = 0;
		const int Distance = 1;
		const int XOffset = 2;
		const int YOffset = 3;
		const int HorizAngle = 4;
		const int VertAngle = 5;
	}
}

namespace Other
{
	const float MatchLength = 0;
}

#endif
