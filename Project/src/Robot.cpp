#include "Timer.h"
#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
                 ultrasonic(0, (5 / 4.88) * (1000 / 25.4), 1), // (5 mm / 4.88 mV) * (1/25.4 in/mm) * (1000 mV/V)
				 driveBase(0, 1)
{
	axisTankLeft = xbox::axis::leftY;
	axisTankRight = xbox::axis::rightY;
	buttonBoost = xbox::btn::lb;
	buttonTurtle = xbox::btn::rb;

	UpdatePreferences();
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
	float speed = 0;
	double distance = ultrasonic.GetDistanceInches();

	if (distance > autoBufferStart + autoBufferLength)
	{
		speed = speedTurtle;
	}
	else if (distance >= autoBufferStart)
	{
		speed = ((distance - autoBufferStart) / autoBufferLength)*(speedTurtle - autoMinSpeed) + autoMinSpeed;
	}
	else
	{
		speed = 0;
	}

	std::cout << "Distance: " << distance << std::endl;

	driveBase.Drive(speed);
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

	driveBase.Drive(leftSpeed * baseSpeed,
					rightSpeed * baseSpeed);
}

void Robot::DisabledInit()
{

}

/*==============================================================
Name: GetGameData
Desc: Gets game data from the field management system that tells
      the robot who owns which sides of the switches and scales.
Parameters:
    data (O) - Three-element boolean array, where each element
               signifies signifies whether you own the left
               (false) or right (true) side of the scale and
               each switch. [Nearest switch, scale, furthest
               switch]
Return value:
    None
==============================================================*/
void Robot::GetGameData(bool data[3])
{
	std::string gameData;
	gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();

	for (int i = 0; i < 3; i++)
	{
		if(gameData[i] == 'L')
		{
			data[i] = false;
		}
		else
		{
			data[i] = true;
		}
	}
}

void Robot::UpdatePreferences()
{
	prefs = Preferences::GetInstance();

	driveBase.SetTrim(prefs->GetDouble("LeftTrim", 1.0),
					  prefs->GetDouble("RightTrim", 1.0));

	speedNormal = prefs->GetFloat("SpeedNormal", 0.5f);
	speedTurtle = prefs->GetFloat("SpeedTurtle", 0.25f);
	speedBoost = prefs->GetFloat("SpeedBoost", 1.0f);
	autoMinSpeed = prefs->GetFloat("AutoMinSpeed", speedTurtle * 0.8);
	autoBufferStart = prefs->GetFloat("AutoBufferStart", 12);
	autoBufferLength = prefs->GetFloat("AutoBufferLength", 24); // distance from start of buffer zone to limit of ultrasonic.
}

START_ROBOT_CLASS(Robot)
