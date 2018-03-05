/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <cmath>
#include <string>
#include <algorithm>

#include "DriveTrain.h"
#include "../RobotMap.h"
#include <RobotDrive.h>


DriveTrain::DriveTrain() : Subsystem("DriveTrain")
{


}

void DriveTrain::InitDefaultCommand()
{
	// Set the default command for a subsystem here.
	// SetDefaultCommand(new MySpecialCommand());
}

// Put methods for controlling this subsystem
// here. Call these from Commands.

void DriveTrain::setMaxSpeed(float speed)
{

}
