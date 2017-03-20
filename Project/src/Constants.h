#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "xBox.h"

namespace Ports
{
#ifndef PRACTICE_2017_ROBOT
	namespace TankDrive
	{
		const int Left = 0;
		const int Right = 1;
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
		const int Left = 0;
		const int Right = 1;
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
	const float SpeedInchesPerSecond = 100;
	const float FullRotationTime = .5;

	enum PosScenario
	{
		Left,
		Middle,
		Right,
		Test
	};

	const PosScenario DefaultScenario = Middle;

	namespace BlindScenarios
	{
		// Timetable format is an array of arrays, each of which is three floats long
		//                                                  (timeLength, leftSpeed, rightSpeed)
		// Namespaces here denote different starting positions
		namespace LeftPos
		{
			const int PeriodCount = 1;
			const float Timetable[PeriodCount][3] = {{4.6,Speeds::DriveBase::Turtle,Speeds::DriveBase::Turtle}};
		}
		namespace MiddlePos
		{
			const int PeriodCount = 1;
			const float Timetable[PeriodCount][3] = {{4.6,Speeds::DriveBase::Turtle,Speeds::DriveBase::Turtle}};

		}
		namespace RightPos
		{
			const int PeriodCount = 1;
			const float Timetable[PeriodCount][3] = {{4.6,Speeds::DriveBase::Turtle,Speeds::DriveBase::Turtle}};
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
		namespace LeftPos
		{
			const int PeriodCount = 1;
			inline void getTimetable(float timeMult, float _timetable[PeriodCount][3])
			{
				float tt[PeriodCount][3] = {{4.6f*timeMult,Speeds::DriveBase::Turtle,Speeds::DriveBase::Turtle}};
				std::copy(&tt[0][0],  &tt[0][0] + PeriodCount * 3, &_timetable[0][0]);
			}
		}
		namespace MiddlePos
		{
			const int PeriodCount = 1;

			inline void getTimetable(float timeMult, float _timetable[PeriodCount][3])
			{
				const float tt[PeriodCount][3] = {{4.6f*timeMult,Speeds::DriveBase::Turtle,Speeds::DriveBase::Turtle}};
				std::copy(&tt[0][0],  &tt[0][0] + PeriodCount * 3, &_timetable[0][0]);
			}
		}
		namespace RightPos
		{
			const int PeriodCount = 1;
			inline void getTimetable(float timeMult, float _timetable[PeriodCount][3])
			{
				float tt[PeriodCount][3] = {{4.6f*timeMult,Speeds::DriveBase::Turtle,Speeds::DriveBase::Turtle}};
				std::copy(&tt[0][0],  &tt[0][0] + PeriodCount * 3, &_timetable[0][0]);
			}
		}
		namespace TestScenario
		{
			const int PeriodCount = 1;
			inline void getTimetable(float timeMult, float _timetable[PeriodCount][3])
			{
				float tt[PeriodCount][3] = {{4.6f*timeMult,Speeds::DriveBase::Turtle,Speeds::DriveBase::Turtle}};
				std::copy(&tt[0][0],  &tt[0][0] + PeriodCount * 3, &_timetable[0][0]);
			}
		}
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
