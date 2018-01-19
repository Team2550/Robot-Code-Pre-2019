#include "Timer.h"
#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
                 ultrasonic(0, (5 / 4.88) * (1000 / 25.4)), // (5 mm / 4.88 mV) * (1/25.4 in/mm) * (1000 mV/V)
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

}

void Robot::AutonomousInit()
{
	autoTimer.Reset();
	autoTimer.Start();
}

void Robot::AutonomousPeriodic()
{
	driveBase.ApplyTrim(SmartDashboard::GetNumber("Left Forwards Ratio", 1.0),
	                    SmartDashboard::GetNumber("Right Forwards Ratio", 1.0),
	                    SmartDashboard::GetNumber("Left Backwards Ratio", 1.0),
	                    SmartDashboard::GetNumber("Right Backwards Ratio", 1.0));
}

void Robot::TeleopInit()
{
	driveBase.Stop();
}

void Robot::TeleopPeriodic()
{
	float leftSpeed = Utility::Deadzone(-driveController.GetRawAxis(axisTankLeft));
	float rightSpeed = Utility::Deadzone(-driveController.GetRawAxis(axisTankRight));

	float baseSpeed = speedNormal;

	if (driveController.GetRawButton(buttonTurtle))
		baseSpeed = speedTurtle;
	else if (driveController.GetRawButton(buttonBoost))
		baseSpeed = speedBoost;

	driveBase.Drive(leftSpeed * baseSpeed,
					rightSpeed * baseSpeed);

	driveBase.ApplyTrim(SmartDashboard::GetNumber("Left Forwards Ratio", 1.0),
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
