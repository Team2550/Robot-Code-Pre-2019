#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "xBox.h"

namespace Ports
{
	namespace TankDrive
	{
		const int Left = 0;
		const int Right = 1;
	}

	namespace Shooter
	{
		const int Motor = 3;
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
	}

	namespace Peripherals
	{
		const int Climb = xbox::btn::lb;
		const int Shoot = xbox::btn::b;
		const int IncreaseShootSpeed = xbox::btn::y;
		const int DecreaseShootSpeed = xbox::btn::x;
	}
}

namespace Speeds
{

}

#endif
