/*
 * Bulldozer.cpp
 *
 *  Created on: Feb 17, 2018
 *      Author: Nathan Walker
 */

#include "Bulldozer.h"

Bulldozer::Bulldozer(int retractPort, int extendPort, int kickInPort, int kickOutPort, double extensionTime) :
	bulldozerSolenoid(extendPort, retractPort), kickerSolenoid(kickOutPort, kickInPort)
{
	bulldozerDelay.Reset();
	bulldozerDelay.Start();

	kickerDelay.Reset();
	kickerDelay.Start();

	bulldozerLastState = UNKNOWN;
	bulldozerPulsing = false;

	kickerLastState = UNKNOWN;
	kicking = false;

	this->extensionTime = extensionTime;
}

void Bulldozer::Reset()
{
	bulldozerLastState = UNKNOWN;
	bulldozerSolenoid.Set(frc::DoubleSolenoid::kOff);
	bulldozerDelay.Reset();
}

void Bulldozer::Extend()
{
	if (bulldozerLastState != EXTEND)
	{
		bulldozerDelay.Reset();
		bulldozerLastState = EXTEND;
	}
	bulldozerPulsing = false;

	if (bulldozerDelay.Get() < extensionTime)
		bulldozerSolenoid.Set(frc::DoubleSolenoid::kForward);
	else
		bulldozerSolenoid.Set(frc::DoubleSolenoid::kOff);
}

void Bulldozer::Retract()
{
	if (bulldozerLastState != RETRACT)
	{
		bulldozerDelay.Reset();
		bulldozerLastState = RETRACT;
	}
	bulldozerPulsing = false;

	if (bulldozerDelay.Get() < extensionTime)
		bulldozerSolenoid.Set(frc::DoubleSolenoid::kReverse);
	else
		bulldozerSolenoid.Set(frc::DoubleSolenoid::kOff);
}

bool Bulldozer::Pulse(double pauseTime)
{
	if (!bulldozerPulsing)
	{
		bulldozerPulsing = true;
		bulldozerDelay.Reset();
		bulldozerLastState = EXTEND;
	}

	if (bulldozerLastState == EXTEND)
	{
		if (bulldozerDelay.Get() < extensionTime)
			bulldozerSolenoid.Set(frc::DoubleSolenoid::kForward);
		else if (bulldozerDelay.Get() < extensionTime + pauseTime)
			bulldozerSolenoid.Set(frc::DoubleSolenoid::kOff);
		else
		{
			bulldozerDelay.Reset();
			bulldozerLastState = RETRACT;
			bulldozerSolenoid.Set(frc::DoubleSolenoid::kReverse);
		}
	}
	else
	{
		if (bulldozerDelay.Get() < extensionTime)
			bulldozerSolenoid.Set(frc::DoubleSolenoid::kReverse);
		else
		{
			bulldozerSolenoid.Set(frc::DoubleSolenoid::kOff);
			bulldozerPulsing = false;
			return true;
		}
	}

	return false;
}

bool Bulldozer::Kick(double pauseTime)
{
	if (!kicking)
	{
		kicking = true;
		kickerDelay.Reset();
		kickerLastState = EXTEND;
	}

	if (kickerLastState == EXTEND)
	{
		if (kickerDelay.Get() < extensionTime)
			kickerSolenoid.Set(frc::DoubleSolenoid::kForward);
		else if (kickerDelay.Get() < extensionTime + pauseTime)
			kickerSolenoid.Set(frc::DoubleSolenoid::kOff);
		else
		{
			kickerDelay.Reset();
			kickerLastState = RETRACT;
			kickerSolenoid.Set(frc::DoubleSolenoid::kReverse);
		}
	}
	else
	{
		if (kickerDelay.Get() < extensionTime)
			kickerSolenoid.Set(frc::DoubleSolenoid::kReverse);
		else
		{
			kickerSolenoid.Set(frc::DoubleSolenoid::kOff);
			kicking = false;
			return true;
		}
	}

	return false;
}
