/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/
#include <WPILib.h>
#include "CommandBase.h"

CommandBase::CommandBase(const char *name, double timeout)
			: Command(name, timeout) {}

CommandBase::CommandBase(const char *name) : Command(name) {}

CommandBase::CommandBase(double timeout) : Command(timeout) {}

CommandBase::CommandBase() : Command() {}

CommandBase::~CommandBase()
{
	delete drivetrain;
	delete oi;
}

void CommandBase::init() {
	drivetrain = new DriveTrain();

	// Subsystem must be instantiated last
	oi = new OI();
}


/* Each subsystem must be initially set to nullptr in order for the static
 * symbols to be available on the roboRIO
 */

DriveTrain* CommandBase::drivetrain = nullptr;
OI* CommandBase::oi = nullptr;
