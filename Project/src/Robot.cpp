#include "Timer.h"
#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
                 ultrasonic(0, (5 / 4.88) * (1000 / 25.4), 1), // (5 mm / 4.88 mV) * (1/25.4 in/mm) * (1000 mV/V)
				 bumperSwitch(0),
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

void Robot::RobotPeriodic()
{
	std::cout << "Limit Switch: " << bumperSwitch.Get() << std::endl;
}

void Robot::AutonomousInit()
{
	UpdatePreferences();

	autoTimer.Reset();
	autoTimer.Start();

	autoTimeHitWall = 0;
	autoHasHitWall = false;
	autoHasReleasedBlock = false;
}

void Robot::AutonomousPeriodic()
{
	double distance = ultrasonic.GetDistanceInches();
	bool bumperTouchingWall = !bumperSwitch.Get();

	// Vision sensing
	if (autoTimer.Get() < 0.5)
	{
		// Set start position based on view from camera
	}
	//==============================================================

	// Drivebase control
	float speed;

	if (autoTimer.Get() < 0.5)
	{
		speed = 0;
	}
	else if (distance > autoBufferStart + autoBufferLength)
	{
		speed = autoMaxSpeed;
	}
	else if (distance >= autoBufferStart)
	{
		// Scale speed down as the robot approaches the wall.
		speed = autoMinSpeed +
				((distance - autoBufferStart) / autoBufferLength)*
				(autoMaxSpeed - autoMinSpeed);
	}
	else if (!bumperTouchingWall)
	{
		speed = autoMinSpeed;
	}
	else
	{
		speed = 0;
	}

	std::cout << "Speed: " << speed << std::endl;

	driveBase.Drive(speed);
	//==============================================================

	// Manipulator control
	if (bumperTouchingWall && !autoHasHitWall)
	{
		autoTimeHitWall = autoTimer.Get();
		autoHasHitWall = true;
	}
	else if (autoHasHitWall && // Robot was against wall since last update.
			 autoTimer.Get() > autoTimeHitWall + 1) // Robot has been against wall for at least 1 second.
	{
		// Get ownership information about switches and scale.
		Position fieldData[3];
		GetGameData(fieldData);

		// Ownership of nearest switch is same as side that robot is on
		if (autoStartPosition == fieldData[0] && !autoHasReleasedBlock)
		{
			// Cause mechanism to release block

			autoHasReleasedBlock = true;
		}
	}
	//==============================================================
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
    data (O) - Three-element array containing which side of each
    		   element is owned by my alliance.
    		   [Nearest switch, scale, farthest switch]
Return value:
    None
==============================================================*/
void Robot::GetGameData(Position data[3])
{
	std::string gameData;
	gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();

	for (int i = 0; i < 3; i++)
	{
		if(gameData[i] == 'L')
		{
			data[i] = left;
		}
		else
		{
			data[i] = right;
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

	if (prefs->GetBoolean("WillStartOnRight", true))
		autoStartPosition = right;
	else
		autoStartPosition = left;

	autoMinSpeed = prefs->GetFloat("AutoMinSpeed", speedTurtle * 0.8);
	autoMaxSpeed = prefs->GetFloat("AutoMaxSpeed", speedTurtle);
	autoBufferStart = prefs->GetFloat("AutoBufferStart", 12);
	autoBufferLength = prefs->GetFloat("AutoBufferLength", 24); // distance from start of buffer zone to limit of ultrasonic.

	std::cout << "Updated Preferences" << std::endl;
}

START_ROBOT_CLASS(Robot)
