#ifndef CONTROLS_H
#define CONTROLS_H

#include "xBox.h"

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
		const int StopShoot = xbox::btn::a;
		const int IncreaseShootSpeed = xbox::btn::y;
		const int DecreaseShootSpeed = xbox::btn::x;
	}
}

#endif
