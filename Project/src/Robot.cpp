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
						   1.13 * (6 * M_PI) / 360), // Multiplied by 1.13 to adjust for incorrect readings
                 autoController(&driveBase, &gyroscope)
{
	axisTankLeft = xbox::axis::leftY;
	axisTankRight = xbox::axis::rightY;
	buttonBoost = xbox::btn::lb;
	buttonTurtle = xbox::btn::rb;

	selectedAutoStrategy = NULL;

	UpdatePreferences();
}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	std::cout << "Calibrating gyro..." << std::endl;
	gyroscope.Calibrate();
	std::cout << "Gyro calibrated" << std::endl;
}

void Robot::RobotPeriodic()
{

}

void Robot::AutonomousInit()
{
	UpdatePreferences();

	if (selectedAutoStrategy != NULL)
	{
		std::cout << "Initializing autonomous" << std::endl;
		autoController.Init(*selectedAutoStrategy);
		autoStrategyCompleted = false;
	}
	else
	{
		autoStrategyCompleted = true;
	}

	autoTimer.Reset();
	autoTimer.Start();
}

void Robot::AutonomousPeriodic()
{
	if (!autoStrategyCompleted)
	{
		autoStrategyCompleted = autoController.Execute();

		if (autoStrategyCompleted)
			std::cout << "Finished at " << autoTimer.Get() << " seconds" << std::endl;
	}
	else
	{
		driveBase.Stop();
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

	// Setup autonomous strategy chooser
	autoStrategyChooser.AddDefault("Auto Line", &AUTO_STRATEGIES::AUTOLINE);
	autoStrategyChooser.AddObject("Right Exchange", &AUTO_STRATEGIES::RIGHTEXCHANGE);
	autoStrategyChooser.AddObject("Left Exchange", &AUTO_STRATEGIES::LEFTEXCHANGE);
	//autoStrategyChooser.AddObject("Auto Line", &AUTO_STRATEGIES::AUTOLINE);
	//autoStrategyChooser.AddObject("Do nothing", &AUTO_STRATEGIES::NOTHING);
	//autoStrategyChooser.AddObject("Look Dang Cool!", &AUTO_STRATEGIES::LOOKDANGCOOL);
	//autoStrategyChooser.AddObject("Rotate", &AUTO_STRATEGIES::ROTATE);
	frc::SmartDashboard::PutData("Autonomous Strategies", &autoStrategyChooser);

	selectedAutoStrategy = autoStrategyChooser.GetSelected();

	std::cout << "Updated Preferences" << std::endl;
}

START_ROBOT_CLASS(Robot)
