#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "xBox.h"

namespace Ports
{
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
	}
}

namespace Controls
{
	namespace TankDrive
	{
		const int Left = xbox::axis::leftY;
		const int Right = xbox::axis::rightY;
		const int Boost = xbox::btn::rb;
		const int Turtle = xbox::btn::lb;
	}

	namespace Peripherals
	{
		const int Climb = xbox::axis::LT;
		const int ClimbToggle = xbox::btn::lb;

		const int Shoot = xbox::btn::b;
		const int ReverseBlender = xbox::btn::rb;
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
		const float Lower = -5;
	}

	namespace Shooter
	{
		const float Shooter = 0.67;
		const float MaxShooter = 1;
		const float CurrentThreshold = 3;
		const float Blender = 0.75;
	}
}

namespace Autonomous
{
	const float SpeedInchesPerSecond = 100;
	const float FullRotationTime = .5;

	// Timetable format is an array of arrays, each of which is three floats long
    //                                                  (timeLength, leftSpeed, rightSpeed)
	const int PeriodCount = 5;
	const float Timetable[PeriodCount][3] = {{93.3 / SpeedInchesPerSecond * 0.8,1,1}, // These need comments for what the values represent
	                                         {FullRotationTime / 2.0,-1,1},
								             {277.4 / SpeedInchesPerSecond * 0.15625,1,1},
	                                         {FullRotationTime / 2.0,-1,1},
	                                         {93.3 / SpeedInchesPerSecond * 0.2,-1,1}};
	// Constants need to be defined to name different periods.
}

namespace UDP
{
	const int DataCount = 5;

	namespace Index
	{
		const int Distance = 0;
		const int XOffset = 1;
		const int YOffset = 2;
		const int HorizAngle = 3;
		const int VertAngle = 4;
	}
}

#endif
