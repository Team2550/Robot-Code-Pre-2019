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
	}
}

namespace Speeds
{
	namespace TankDrive
	{
		const float Normal = 0.4;
		const float Boost = 0.8;
		const float Turtle = 0.25;
	}

	namespace Shooter
	{
		const float ShooterSpeed = 0.82;
		const float BlenderSpeed = 0.75;
	}
}

#endif
