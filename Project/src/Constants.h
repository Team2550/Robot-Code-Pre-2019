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
		const int Climb = xbox::btn::lb;
		const int ClimbDown = xbox::axis::LT;
		const int Shoot = xbox::btn::b;
		const int ReverseBlender = xbox::btn::rb;
		const int IncreaseShootSpeed = xbox::btn::y;
		const int DecreaseShootSpeed = xbox::btn::x;
		const int autoAim = xbox::btn::a;
	}
}

namespace Speeds
{
	namespace TankDrive
	{
		const float Normal = -0.4;
		const float Boost = -1;
		const float Turtle = -0.25;
	}

	namespace Shooter
	{
		const float ShooterSpeed = 0.65;
		const float BlenderSpeed = 1.0;
	}
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

namespace Other
{
	const float MatchLength = 0;
}

#endif
