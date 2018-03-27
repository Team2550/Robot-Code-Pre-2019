#include "Clamp.h"

Clamp::Clamp(int armUpPort, int armDownPort, int closePort, int openPort) :
	armSolenoid(armDownPort, armUpPort), clampSolenoid(openPort, closePort)
{
	armSolenoid.Set(DoubleSolenoid::kOff);
	clampSolenoid.Set(DoubleSolenoid::kOff);
}

void Clamp::ArmUp()
{
	armSolenoid.Set(DoubleSolenoid::kReverse);
}

void Clamp::ArmDown()
{
	armSolenoid.Set(DoubleSolenoid::kForward);
}

void Clamp::Close()
{
	clampSolenoid.Set(DoubleSolenoid::kReverse);
}

void Clamp::Open()
{
	clampSolenoid.Set(DoubleSolenoid::kForward);
}

bool Clamp::GetIsDown()
{
	return armSolenoid.Get() == DoubleSolenoid::kForward;
}

bool Clamp::GetIsOpen()
{
	return clampSolenoid.Get() == DoubleSolenoid::kForward;
}


