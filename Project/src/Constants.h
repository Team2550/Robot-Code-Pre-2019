#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "xBox.h"

namespace Ports
{
#ifndef PRACTICE_2017_ROBOT
	namespace TankDrive
	{
		const int Left = 1;
		const int Right = 0;
	}

	namespace Shooter
	{
		const int Motor = 3;
		const int BlenderMotor = 4;
	}

	namespace Lifter
	{
		const int Motor = 2;
	}

	namespace PDP
	{
		const int Shooter = 2;
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

	namespace Shooter
	{
		const int Motor = 4;
		const int BlenderMotor = 3;
	}

	namespace Lifter
	{
		const int Motor = 2;
	}

	namespace PDP
	{
		const int Shooter = 2;
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

	namespace Peripherals
	{
		const int Climb = xbox::axis::LT;
		const int ClimbToggle = xbox::btn::lb;

		const int Shoot = xbox::btn::b;
		const int Blender = xbox::btn::a;
		const int ReverseBlender = xbox::axis::RT;
		const int IncreaseShootSpeed = xbox::btn::y;
		const int DecreaseShootSpeed = xbox::btn::x;
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

	namespace Lift
	{
		const float Lift = 1;
		const float Lower = -1;
	}

	namespace Shooter
	{
		const float Shooter = 0.65;
		const float MaxShooter = 1;
		const float CurrentThreshold = 3;
		const float Blender = 0.75;
	}
}

namespace Autonomous
{
	enum Ready // What auto functions are ready
	{
		Safe, // Safe mode (pass baseline) is ready
		Blind, // Putting gear on peg blindly is ready
		Vision // Putting gear on peg with vision is ready
	};

	enum Scenario
	{
		Middle,
		Side
	};

	const Scenario DefaultScenario = Middle;

	namespace BlindTimes
	{
		const float Middle = 4.6;
		const float Side = 6.9;
	}
}

namespace UDP
{
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
