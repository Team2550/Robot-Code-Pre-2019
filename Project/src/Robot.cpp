#include "Timer.h"
#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
                 driveBase()
{

}

Robot::~Robot()
{

}

void Robot::RobotInit()
{

}

void Robot::AutonomousInit()
{
	autoTimer.Reset();
	autoTimer.Start();
}

void Robot::AutonomousPeriodic()
{
	driveBase.applyTrim(SmartDashboard::GetNumber("Left Forwards Ratio", 1.0),
	                    SmartDashboard::GetNumber("Right Forwards Ratio", 1.0),
	                    SmartDashboard::GetNumber("Left Backwards Ratio", 1.0),
	                    SmartDashboard::GetNumber("Right Backwards Ratio", 1.0));
}

void Robot::TeleopInit()
{
	driveBase.stop();
}

void Robot::TeleopPeriodic()
{
	float leftSpeed = Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Left));
	float rightSpeed = Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Right));

	float speed = Speeds::DriveBase::Normal;

	if (driveController.GetRawButton(Controls::TankDrive::Turtle))
		speed = Speeds::DriveBase::Turtle;
	else if (driveController.GetRawButton(Controls::TankDrive::Boost))
		speed = Speeds::DriveBase::Boost;

	driveBase.drive(leftSpeed * speed,
					rightSpeed * speed);

	driveBase.applyTrim(SmartDashboard::GetNumber("Left Forwards Ratio", 1.0),
	                    SmartDashboard::GetNumber("Right Forwards Ratio", 1.0),
	                    SmartDashboard::GetNumber("Left Backwards Ratio", 1.0),
	                    SmartDashboard::GetNumber("Right Backwards Ratio", 1.0));
}

void Robot::DisabledInit()
{

}

void Robot::ClearSmartDashboard()
{

}

START_ROBOT_CLASS(Robot)
