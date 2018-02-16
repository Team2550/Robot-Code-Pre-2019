#include "Timer.h"
#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
                 solenoid(1, 0), driveBase(0, 1)
{
	speedNormal = 0.5f;
	speedTurtle = 0.25f;
	speedBoost = 1.0f;

	axisTankLeft = xbox::axis::leftY;
	axisTankRight = xbox::axis::rightY;
	buttonBoost = xbox::btn::lb;
	buttonTurtle = xbox::btn::rb;
	buttonSolenoidToggle = xbox::btn::rb;

	pneumaticTimeStamp = 0;
	solenoidToggle = false;

	prefs = Preferences::GetInstance();
}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	UpdatePreferences();

	pneumaticDelay.Reset();
	pneumaticDelay.Start();


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
	float leftSpeed = Utility::Deadzone(-driveController.GetRawAxis(axisTankLeft));
	float rightSpeed = Utility::Deadzone(-driveController.GetRawAxis(axisTankRight));

	float baseSpeed = speedNormal;

	if (driveController.GetRawButton(buttonTurtle))
		baseSpeed = speedTurtle;
	else if (driveController.GetRawButton(buttonBoost))
		baseSpeed = speedBoost;

	//When the right bumper is pressed, the solenoid is turned on for .5 sec then turned off and the controller rubles on the right side for the duration of the if statement
	if (perifController.GetRawButton(buttonSolenoidToggle))
	{
		solenoidToggle = true;
		pneumaticDelay.Reset();
	}

	if (solenoidToggle)
	{
		if (pneumaticDelay.Get() < 0.5)
			solenoid.Set(frc::DoubleSolenoid::kForward);
		else if (pneumaticDelay.Get() < 1)
			solenoid.Set(frc::DoubleSolenoid::kOff);
		else if (pneumaticDelay.Get() < 1.5)
			solenoid.Set(frc::DoubleSolenoid::kReverse);
		else
		{
			solenoid.Set(frc::DoubleSolenoid::kOff);
			solenoidToggle = false;
		}
	}

	driveBase.Drive(leftSpeed * baseSpeed,
					rightSpeed * baseSpeed);
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
