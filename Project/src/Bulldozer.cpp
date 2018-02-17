/*
 * Bulldozer.cpp
 *
 *  Created on: Feb 17, 2018
 *      Author: Nathan Walker
 */

#include "Bulldozer.h"

Bulldozer::Bulldozer(int retractPort, int extendPort, double extensionTime) : solenoid(extendPort, retractPort)
{
	delay.Reset();
	delay.Start();

	lastState = RETRACT;
	pulsing = false;
	this->extensionTime = extensionTime;
}

void Bulldozer::Extend()
{
	if (lastState != EXTEND)
	{
		delay.Reset();
		lastState = EXTEND;
	}
	pulsing = false;

	if (delay.Get() < extensionTime)
		solenoid.Set(frc::DoubleSolenoid::kForward);
	else
		solenoid.Set(frc::DoubleSolenoid::kOff);
}

void Bulldozer::Retract()
{
	if (lastState != RETRACT)
	{
		delay.Reset();
		lastState = RETRACT;
	}
	pulsing = false;

	if (delay.Get() < extensionTime)
		solenoid.Set(frc::DoubleSolenoid::kReverse);
	else
		solenoid.Set(frc::DoubleSolenoid::kOff);
}

bool Bulldozer::Pulse(double pauseTime)
{
	if (!pulsing)
	{
		pulsing = true;
		delay.Reset();
		lastState = EXTEND;
	}

	if (lastState == EXTEND)
	{
		if (delay.Get() < extensionTime)
			solenoid.Set(frc::DoubleSolenoid::kForward);
		else if (delay.Get() < extensionTime + pauseTime)
			solenoid.Set(frc::DoubleSolenoid::kOff);
		else
		{
			delay.Reset();
			lastState = RETRACT;
			solenoid.Set(frc::DoubleSolenoid::kReverse);
		}
	}
	else
	{
		if (delay.Get() < extensionTime)
			solenoid.Set(frc::DoubleSolenoid::kReverse);
		else
		{
			solenoid.Set(frc::DoubleSolenoid::kOff);
			return true;
		}
	}

	return false;
}
