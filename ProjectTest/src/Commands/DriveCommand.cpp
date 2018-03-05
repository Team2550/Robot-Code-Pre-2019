/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "DriveCommand.h"


DriveCommand::DriveCommand(const double speed) : Command("Drive"), m_speed(speed)
{
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(Robot::chassis.get());
	Requires(driveTrain);
}

// Called just before this Command runs the first time
void DriveCommand::Initialize()
{
	//driveTrain->setMaxSpeed(m_speed);
	//SmartDashboard::PutBoolean("Testing Boolean On", m_speed == Config::DriveTrain::preciseModeMaxSpeed);
}

// Called repeatedly when this Command is scheduled to run
void DriveCommand::Execute()
{
	/*drivetrain->move
		(
			oi->getAxisValue(logicalAxes::DriveX),
			oi->getAxisValue(logicalAxes::DriveY),
			oi->getAxisValue(logicalAxes::DriveRotate) / 2
		);v*/
}

// Make this return true when this Command no longer needs to run execute()
bool DriveCommand::IsFinished()
{
	return false;
}

// Called once after isFinished returns true
void DriveCommand::End()
{

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveCommand::Interrupted()
{

}
