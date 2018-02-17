#include "Timer.h"
#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
                 driveBase(0, 1), bulldozer(0, 1, 0.5)
{
	speedNormal = 0.5f;
	speedTurtle = 0.25f;
	speedBoost = 1.0f;

	axisTankLeft = xbox::axis::leftY;
	axisTankRight = xbox::axis::rightY;
	buttonBoost = xbox::btn::lb;
	buttonTurtle = xbox::btn::rb;
	buttonBulldozerExtend = xbox::btn::rb;

	prefs = Preferences::GetInstance();
}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	UpdatePreferences();
}

void Robot::AutonomousInit()
{
	UpdatePreferences();

	autoTimer.Reset();
	autoTimer.Start();
}

void Robot::AutonomousPeriodic()
{

}

void Robot::TeleopInit()
{
	UpdatePreferences();

	driveBase.Stop();
}

void Robot::TeleopPeriodic()
{
	// Drivebase
	float leftSpeed = Utility::Deadzone(-driveController.GetRawAxis(axisTankLeft));
	float rightSpeed = Utility::Deadzone(-driveController.GetRawAxis(axisTankRight));

	float baseSpeed = speedNormal;

	if (driveController.GetRawButton(buttonTurtle))
		baseSpeed = speedTurtle;
	else if (driveController.GetRawButton(buttonBoost))
		baseSpeed = speedBoost;

	driveBase.Drive(leftSpeed * baseSpeed,
					rightSpeed * baseSpeed);

	// Bulldozer
	if (perifController.GetRawButton(buttonBulldozerExtend))
		bulldozer.Extend();
	else
		bulldozer.Retract();
}

void Robot::DisabledInit()
{

}

void Robot::UpdatePreferences()
{
	prefs = Preferences::GetInstance();
	driveBase.SetTrim(prefs->GetDouble("LeftTrim", 1.0),
					  prefs->GetDouble("RightTrim", 1.0));
}

START_ROBOT_CLASS(Robot)
