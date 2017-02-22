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
		const int flip = xbox::btn::a;
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
	namespace TankDrive
	{
		const float Normal = 0.5;
		const float Boost = 1;
		const float Turtle = 0.25;
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
	// Timetable format is an array of arrays, each of which is three floats long
    //                                                  (timeLength, leftSpeed, rightSpeed)
	const int PeriodCount = 3;
	const float Timetable[PeriodCount][3] = {{2,1,1},
	                                         {0.5,-1,1},
								             {1,1,1}};
}


namespace UDP
{
	const int DataCount = 5;
}

#endif
