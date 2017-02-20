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
		const int AutoAim = xbox::btn::a;
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
		const float ShooterSpeed = 0.67;
		const float BlenderSpeed = 0.75;
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

#endif
