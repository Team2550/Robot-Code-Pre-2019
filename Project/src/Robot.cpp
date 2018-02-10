#include "Timer.h"
#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
                 ultrasonic(0, (5 / 4.88) * (1000 / 25.4), 1), // (5 mm / 4.88 mV) * (1/25.4 in/mm) * (1000 mV/V)
                 bumperSwitch(0, LimitSwitch::LOW),
				 gyroscope(frc::SPI::Port::kOnboardCS0),
				 driveBase(0, 1, 0, 1, 2, 3,
						   (6 * M_PI) / 360, // (circumference / 360 pulses per rotation)
						   1.13 * (6 * M_PI) / 360) // Multiplied by 1.5 to adjust for incorrect readings
{
	axisTankLeft = xbox::axis::leftY;
	axisTankRight = xbox::axis::rightY;
	buttonBoost = xbox::btn::lb;
	buttonTurtle = xbox::btn::rb;

	autoChooser.AddDefault("Default", DEFAULT);
	autoChooser.AddObject("Disable", DISABLE);
	frc::SmartDashboard::PutData("Autonomous Strategies", &autoChooser);

	UpdatePreferences();
}

Robot::~Robot()
{

}

void Robot::RobotInit()
{

	gyroscope.Calibrate();
}

void Robot::RobotPeriodic()
{

}

void Robot::AutonomousInit()
{
	UpdatePreferences();

	gyroscope.Reset();
	driveBase.ResetDistance();

	autoTimer.Reset();
	autoTimer.Start();

	autoStage = 0;
}

void Robot::AutonomousPeriodic()
{
	switch( autoStrategy )
	{
	case DEFAULT:
		std::cout << "Default Auto Strategy" << std::endl;

		switch( autoStage )
		{
		case 0:
			if ( AutoRotate(90, 5, speedTurtle) )
			{
				autoStage++;
				driveBase.ResetDistance();
			}
			break;

		case 1:
			if ( AutoDrive(48, 90, speedTurtle) )
			{
				autoStage++;
			}
			break;

		default:
			driveBase.Stop();
			break;
		}
		driveBase.Stop();
		break;

	case DISABLE:
		std::cout << "Auto Disabled" << std::endl;
		driveBase.Stop();
		break;
	}
}

void Robot::TeleopInit()
{
	UpdatePreferences();

	gyroscope.Reset();

	driveBase.Stop();
	driveBase.ResetDistance();
}

void Robot::TeleopPeriodic()
{
	std::cout << "Left: " << std::setw(5) << driveBase.GetLeftDistance() << ' '
	          << "Right: " << std::setw(5) << driveBase.GetRightDistance() << std::endl;

	int controllerPOV = driveController.GetPOV();

	if (controllerPOV == 315)
	{
		driveBase.Drive(0, speedTurtle);
	}
	else if (controllerPOV == 0)
	{
		driveBase.Drive(speedTurtle);
	}
	else if (controllerPOV == 45)
	{
		driveBase.Drive(speedTurtle, 0);
	}
	else if (controllerPOV == 135)
	{
		driveBase.Drive(0, -speedTurtle);
	}
	else if (controllerPOV == 180)
	{
		driveBase.Drive(-speedTurtle);
	}
	else if (controllerPOV == 225)
	{
		driveBase.Drive(-speedTurtle, 0);
	}
	else
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
			data[i] = LEFT;
		}
		else
		{
			data[i] = RIGHT;
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
		autoStartPosition = RIGHT;
	else
		autoStartPosition = LEFT;

	autoMinSpeed = prefs->GetFloat("AutoMinSpeed", speedTurtle * 0.8);
	autoMaxSpeed = prefs->GetFloat("AutoMaxSpeed", speedTurtle);
	autoBufferStart = prefs->GetFloat("AutoBufferStart", 12);
	autoBufferLength = prefs->GetFloat("AutoBufferLength", 24); // distance from start of buffer zone to limit of ultrasonic.

	autoStrategy = autoChooser.GetSelected();

	std::cout << "Updated Preferences" << std::endl;
}

bool Robot::AutoDrive(double targetDist, double targetAngle, double speed)
{
// OPTION ONE
	// Get the angle that the robot has drifted from its target as a percentage out of 90 degrees
	double angleOffsetPercent = (gyroscope.GetAngle() - targetAngle) / 90;

	// Limit angle offset to range of -90 to 90 (-100% to 100%)
	if (angleOffsetPercent > 1)
		angleOffsetPercent = 1;
	if (angleOffsetPercent < -1)
		angleOffsetPercent = -1;

	// Set speed offset to the speed multiplied by the percent that the robot is to 90 degrees off target
	double rightSpeedOffset = angleOffsetPercent * speed;

	driveBase.Drive(speed - rightSpeedOffset, speed + rightSpeedOffset);

	// End Condition (Average distance between left and right is greater than target)
	return 0.5 * (driveBase.GetLeftDistance() + driveBase.GetRightDistance()) > targetDist;

// OPTION TWO
/*	double angle = gyroscope.GetAngle();

	if (angle > targetAngle + 1)
	{
		double speedOffset = speed - speed / abs(angle - targetAngle);
		driveBase.Drive(speed, speed + speedOffset);
	}
	else if (angle < targetAngle - 1)
	{
		double speedOffset = speed - speed / abs(angle - targetAngle);
		driveBase.Drive(speed + speedOffset, speed);
	}
	else
	{
		driveBase.Stop();
	}

	// End Condition (Average distance between left and right is greater than target)
	return 0.5 * (driveBase.GetLeftDistance() + driveBase.GetRightDistance()) > targetDist;*/
}

bool Robot::AutoRotate(double targetAngle, double threshold, double speed)
{
	if ( gyroscope.GetAngle() > targetAngle + threshold )
		driveBase.Drive(speed, -speed);
	else if ( gyroscope.GetAngle() < targetAngle - threshold )
		driveBase.Drive(-speed, speed);

	// End Condition (Angle is within range of (targetAngle - threshold, targetAngle + threshold))
	return abs( gyroscope.GetAngle() - targetAngle ) < threshold;
}

START_ROBOT_CLASS(Robot)
