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
		const int Blender = xbox::btn::a;
		const int ReverseBlender = xbox::btn::rb;
		const int IncreaseShootSpeed = xbox::btn::y;
		const int DecreaseShootSpeed = xbox::btn::x;
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

	namespace Lift
	{
		const float Lift = 1;
		const float Lower = -1;
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

	enum safeMode
	{
		on,
		off
	};

	const safeMode DefaultMode = off;
	enum PosScenario
	{
		FarLeft,
		SafeFarLeft,
		Middle,
		SafeMiddle,
		MidRight,
		SafeMidRight,
		FarRight,
		SafeFarRight,
		Test
	};

	const PosScenario DefaultScenario = FarLeft;

	namespace BlindScenarios
	{
		// Timetable format is an array of arrays, each of which is three floats long
		//                                                  (timeLength, leftSpeed, rightSpeed)
		// Namespaces here denote different starting positions
		namespace FarLeftPos
		{
			const int PeriodCount = 1;
			const float Timetable[PeriodCount][3] = {{0,0,0}};
		}
		namespace SafeFarLeftPos
		{
			const int PeriodCount = 1;
			const float Timetable[PeriodCount][3] = {{185.3 / SpeedInchesPerSecond,0.5,0.5}};
		}

		namespace MiddlePos
		{
			const int PeriodCount = 1;
			const float Timetable[PeriodCount][3] = {{90 / SpeedInchesPerSecond,0.5,0.5}};
		}

		namespace SafeMiddle
		{
			const int PeriodCount = 6;
			const float Timetable[PeriodCount][3] = {{90 / SpeedInchesPerSecond,0.5,0.5},
					 	 	 	 	 	 	 	 	 {FullRotationTime / 2.0,-0.5,0.5},
													 {60 / SpeedInchesPerSecond,0.5,0.5},
													 {90 / SpeedInchesPerSecond,0.5,0.5},
													 {FullRotationTime / 2.0,0.5,-0.5},
													 {90 / SpeedInchesPerSecond,0.5,0.5}};
		}
		namespace MidRightPos
		{
			const int PeriodCount = 1;
			const float Timetable[PeriodCount][3] = {{0,0,0}};
		}
		namespace SafeMidRightPos
		{
			const int PeriodCount = 6;
			const float Timetable[PeriodCount][3] = {{90 / SpeedInchesPerSecond,0.5,0.5},
								 	 	 	         {FullRotationTime / 2.0,0.5,-0.5},
													 {60 / SpeedInchesPerSecond,0.5,0.5},
													 {90 / SpeedInchesPerSecond,0.5,0.5},
													 {FullRotationTime / 2.0,-0.5,0.5},
													 {90 / SpeedInchesPerSecond,0.5,0.5}};
		}

		namespace FarRightPos
		{
			const int PeriodCount = 3;
			const float Timetable[PeriodCount][3] = {{90 / SpeedInchesPerSecond,0.5,0.5},
													 {FullRotationTime / 4.0,-0.5,0.5},
													 {277.4 / SpeedInchesPerSecond * 0.15625,0.5,0.5}};
		}
	}

	namespace DynamicBlindScenarios
	{
		// These scenarios are mode of inline functions that are designed to generate the timetables based on variables that can be changed live.
		// These should only be used when testing for figuring out values.
		/*=================================================
			Name: timetable
			Desc: Initializes a timetable array to be used by a choreographer.
			Arguments:
				speedInchesPerSecond (I) : The max speed of the robot in inches per second
				fullRotationTime (I)     : The amount of time in seconds it takes the robot to turn 180 degrees at full speed
				_timetable (O)           : The array to initialize as a timetable
			Return:
				none
		=================================================*/
		namespace FarLeftPos
		{
			const int PeriodCount = 1;
			inline void getTimetable(float speedInchesPerSecond, float fullRotationTime, float _timetable[PeriodCount][3])
			{
				float tt[PeriodCount][3] = {{0,0,0}};
				std::copy(&tt[0][0],  &tt[0][0] + PeriodCount * 3, &_timetable[0][0]);
			}
		}
		namespace SafeFarLeftPos
		{
			const int PeriodCount = 1;
			inline void getTimetable(float speedInchesPerSecond , float fullRotationTime, float _timetable[PeriodCount][3])
			{
				float tt[PeriodCount][3] = {{185 / speedInchesPerSecond,0.5,0.5}};
				std::copy(&tt[0][0],  &tt[0][0] + PeriodCount * 3, &_timetable[0][0]);

			}
		}

		namespace MiddlePos
		{
			const int PeriodCount = 1;
			inline void getTimetable(float speedInchesPerSecond, float fullRotationTime, float _timetable[PeriodCount][3])
			{
				float tt[PeriodCount][3] = {{90 / speedInchesPerSecond,0.5,0.5}};
				std::copy(&tt[0][0],  &tt[0][0] + PeriodCount * 3, &_timetable[0][0]);
			}
		}
		namespace SafeMiddlePos
		{
			const int PeriodCount = 6;
			inline void getTimetable(float speedInchesPerSecond, float fullRotationTime, float _timetable[PeriodCount][3])
			{
				float tt[PeriodCount][3] = {{90 / SpeedInchesPerSecond,0.5,0.5},
							 	 	 	 	{FullRotationTime / 2.0,-0.5,0.5},
											{60 / SpeedInchesPerSecond,0.5,0.5},
											{90 / SpeedInchesPerSecond,0.5,0.5},
											{FullRotationTime / 4.0,0.5,-0.5},
											{90 / SpeedInchesPerSecond,0.5,0.5}};

		 	 	std::copy(&tt[0][0],  &tt[0][0] + PeriodCount * 3, &_timetable[0][0]);
			}
		}

		namespace MidRightPos
		{
			const int PeriodCount = 1;
			inline void getTimetable(float speedInchesPerSecond, float fullRotationTime, float _timetable[PeriodCount][3])
			{
				float tt[PeriodCount][3] = {{0,0,0}};
				std::copy(&tt[0][0], &tt[0][0] + PeriodCount * 3, &_timetable[0][0]);
			}
		}
		namespace SafeMidRightPos
		{
			const int PeriodCount = 6;
			inline void getTimetable(float speedInchesPerSecond, float fullRotationTime, float _timetable[PeriodCount][3])

			{
				float tt[PeriodCount][3] = {{90 / SpeedInchesPerSecond,0.5,0.5},
									 	 	{FullRotationTime / 2.0,0.5,-0.5},
											{60 / SpeedInchesPerSecond,0.5,0.5},
											{90 / SpeedInchesPerSecond,0.5,0.5},
											{FullRotationTime / 4.0,-0.5,0.5},
											{90 / SpeedInchesPerSecond,0.5,0.5}};

				std::copy(&tt[0][0],  &tt[0][0] + PeriodCount * 3, &_timetable[0][0]);
			}
		}

		namespace FarRightPos
		{
			const int PeriodCount = 3;
			inline void getTimetable(float speedInchesPerSecond, float fullRotationTime, float _timetable[PeriodCount][3])
			{
				float tt[PeriodCount][3] = {{90 / speedInchesPerSecond,0.5,0.5},
			                                {fullRotationTime / 4,0.5,-0.5},
										    {277.4f / speedInchesPerSecond * 0.15625f,0.5,0.5}};
				std::copy(&tt[0][0],  &tt[0][0] + PeriodCount * 3, &_timetable[0][0]);
			}
		}

		namespace TestScenario
		{
			const int PeriodCount = 1;
			inline void getTimetable(float speedInchesPerSecond, float fullRotationTime, float _timetable[PeriodCount][3])
			{
				float tt[PeriodCount][3] = {{2, 1, 1}};
				std::copy(&tt[0][0],  &tt[0][0] + PeriodCount * 3, &_timetable[0][0]);
			}
		}
	}
}

namespace UDP
{
	const int DataCount = 5;

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
