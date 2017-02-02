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
	bool doLiftDown = perifController.GetRawAxis(Controls::Peripherals::ClimbDown) > 0.5;

	if(doLift)
		liftMotor.Set(1.0);
	else if(doLiftDown)
		liftMotor.Set(-1.0);
	else
		liftMotor.Set(0);
}
