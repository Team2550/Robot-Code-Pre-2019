/*
 * Bulldozer.cpp
 *
 *  Created on: Feb 17, 2018
 *      Author: Nathan Walker
 */

#include "Bulldozer.h"

Bulldozer::Bulldozer(int retractPort, int extendPort, int kickInPort, int kickOutPort) :
	bulldozerSolenoid(extendPort, retractPort), kickerSolenoid(kickOutPort, kickInPort)
{
	pulseTimer.Reset();
	pulseTimer.Start();

	kickTimer.Reset();
	kickTimer.Start();

	bulldozerSolenoid.Set(DoubleSolenoid::kOff);
	kickerSolenoid.Set(frc::DoubleSolenoid::kReverse);

	isPulsing = false;
	isKicking = false;
}

void Bulldozer::Extend()
{
	isPulsing = false;
	bulldozerSolenoid.Set(DoubleSolenoid::kForward);
}

void Bulldozer::Retract()
{

	isPulsing = false;
	bulldozerSolenoid.Set(DoubleSolenoid::kReverse);
}

bool Bulldozer::GetIsExtended()
{
	return bulldozerSolenoid.Get() == DoubleSolenoid::kForward;
}

bool Bulldozer::Pulse(double waitTimeSec)
{
	if (!isPulsing)
	{
		pulseTimer.Reset();
		isPulsing = true;
	}

	if (pulseTimer.Get() < waitTimeSec)
	{
		bulldozerSolenoid.Set(DoubleSolenoid::kForward);
	}
	else if (pulseTimer.Get() < waitTimeSec * 2)
	{
		bulldozerSolenoid.Set(DoubleSolenoid::kReverse);
	}
	else
	{
		bulldozerSolenoid.Set(DoubleSolenoid::kReverse);
		isPulsing = false;
		return true;
	}

	return false;
}

bool Bulldozer::Kick(double waitTimeSec)
{
	if (!isKicking)
	{
		kickTimer.Reset();
		isKicking = true;
	}

	if (kickTimer.Get() < waitTimeSec)
	{
		kickerSolenoid.Set(DoubleSolenoid::kForward);
	}
	else if (kickTimer.Get() < waitTimeSec * 2)
	{
		kickerSolenoid.Set(DoubleSolenoid::kReverse);
	}
	else
	{
		kickerSolenoid.Set(DoubleSolenoid::kReverse);
		isKicking = false;
		return true;
	}

	if (kickerSolenoid.Get() == DoubleSolenoid::kForward)
		std::cout << "Forward\n";
	else if (kickerSolenoid.Get() == DoubleSolenoid::kReverse)
		std::cout << "Reverse\n";
	else
		std::cout << "Off\n";

	return false;
}
