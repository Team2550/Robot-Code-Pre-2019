/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <Commands/Subsystem.h>
#include <RobotDrive.h>
#include <Victor.h>

#include <vector>
#include <memory>

class DriveTrain : public frc::Subsystem
{


public:
	DriveTrain();
	void InitDefaultCommand() override;

	void setMaxSpeed(float speed);

private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities

	std::vector<std::shared_ptr<Victor>> leftMotor;
	std::vector<std::shared_ptr<Victor>> rightMotor;

protected:
	float m_maxSpeed;
};

