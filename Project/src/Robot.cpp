#include "Timer.h"
#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
                 ultrasonic(0, (5 / 4.88) * (1000 / 25.4), 1), // (5 mm / 4.88 mV) * (1/25.4 in/mm) * (1000 mV/V)
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
	double distance = ultrasonic.GetDistanceInches();

	if (distance > 45)
		driveBase.Drive(speedTurtle);
	else if (distance > 35)
		driveBase.Drive(speedTurtle * 0.5);
	else if (distance < 15)
		driveBase.Drive(-speedTurtle);
	else if (distance < 25)
		driveBase.Drive(-speedTurtle * 0.5);
	else
		driveBase.Stop();

	std::cout << ultrasonic.GetDistanceInches() << std::endl;

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

void Robot::ClearSmartDashboard()
{

}

START_ROBOT_CLASS(Robot)
