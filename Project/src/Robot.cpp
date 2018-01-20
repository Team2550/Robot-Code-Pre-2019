#include "Timer.h"
#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
                 driveBase(0, 1)
{
	speedNormal = 0.5f;
	speedTurtle = 0.25f;
	speedBoost = 1.0f;

	axisTankLeft = xbox::axis::leftY;
	axisTankRight = xbox::axis::rightY;
	buttonBoost = xbox::btn::lb;
	buttonTurtle = xbox::btn::rb;
}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	driveBase.SetTrim(SmartDashboard::GetNumber("Left Trim", 1.0),
	                  SmartDashboard::GetNumber("Right Trim", 1.0));
}

void Robot::AutonomousInit()
{
	autoTimer.Reset();
	autoTimer.Start();
}

void Robot::AutonomousPeriodic()
{
	driveBase.SetTrim(SmartDashboard::GetNumber("Left Trim", 1.0),
	                  SmartDashboard::GetNumber("Right Trim", 1.0));
}

void Robot::TeleopInit()
{
	driveBase.Stop();
}

void Robot::TeleopPeriodic()
{
	driveBase.SetTrim(SmartDashboard::GetNumber("Left Trim", 1.0),
	                  SmartDashboard::GetNumber("Right Trim", 1.0));

	float leftSpeed = Utility::Deadzone(-driveController.GetRawAxis(axisTankLeft));
	float rightSpeed = Utility::Deadzone(-driveController.GetRawAxis(axisTankRight));

	float baseSpeed = speedNormal;

	if (driveController.GetRawButton(buttonTurtle))
		baseSpeed = speedTurtle;
	else if (driveController.GetRawButton(buttonBoost))
		baseSpeed = speedBoost;

	driveBase.Drive(leftSpeed * baseSpeed,
					rightSpeed * baseSpeed);
}

void Robot::DisabledInit()
{

}

void Robot::ClearSmartDashboard()
{

}

START_ROBOT_CLASS(Robot)
