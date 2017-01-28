#include "Lift.h"

Lift::Lift(Joystick& _driveController, Joystick& _perifController) :
		   driveController(_driveController), perifController(_perifController),
		   liftMotor(Ports::Lifter::Motor)
{

}

void Lift::RobotInit()
{

}

void Lift::AutoInit()
{

}

void Lift::AutoPeriodic()
{

}

void Lift::TeleopInit()
{

}

void Lift::TeleopPeriodic()
{
	bool doLift = perifController.GetRawButton(Controls::Peripherals::Climb);

	liftMotor.Set(doLift ? 1.0 : 0.0);
}
