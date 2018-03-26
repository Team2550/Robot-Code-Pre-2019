#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
				 autoController(&driveBase, &bulldozer, &gyroscope),
                 ultrasonic(0, (5 / 4.88) * (1000 / 25.4), 1), // (5 mm / 4.88 mV) * (1/25.4 in/mm) * (1000 mV/V)
				 gyroscope(frc::SPI::Port::kOnboardCS0),
				 driveBase(0, 1, 0, 1, 2, 3, 6.07 * M_PI, 512), // Pulses per rotation is set by encoder DIP switch. 512 PPR uses DIP switch configuration 0001.
				 bulldozer(0, 1, 4, 5),
				 clamp(2, 3, 6, 7) // Port numbers are temporary and not based on robot's wiring
{
	axisTankLeft = xbox::axis::leftY;
	axisTankRight = xbox::axis::rightY;
	buttonBoost = xbox::btn::rb;
	buttonTurtle = xbox::btn::lb;
	buttonBulldozerExtend = xbox::btn::rb;
	buttonBulldozerPulse = xbox::btn::lb;
	buttonBulldozerKick = xbox::btn::a;
	buttonArmToggle = xbox::btn::y;
	buttonClampToggle = xbox::btn::x;

	bulldozerPulseToggle = false;

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

	// Start Video Stream
	CameraServer::GetInstance()->StartAutomaticCapture();
}

void Robot::AutonomousInit()
{
	UpdatePreferences();

	driveBase.Stop();

	autoTimer.Reset();
	autoTimer.Start();

	if (selectedAutoStrategy != NULL)
	{
		std::cout << "Initializing autonomous" << std::endl;
		autoController.Init(*selectedAutoStrategy);
		autoStrategyCompleted = false;
	}
	else
	{
		std::cout << "Selected strategy is blank" << std::endl;
		autoStrategyCompleted = true;
	}
}

void Robot::AutonomousPeriodic()
{
	if (autoTimer.Get() >= autoDelay && !autoStrategyCompleted)
	{
		std::cout << "Left: " << std::setw(5) << driveBase.GetLeftDistance() << ' '
		          << "Right: " << std::setw(5) << driveBase.GetRightDistance() << ' '
				  << "Angle: " << std::setw(5) << gyroscope.GetAngle() << std::endl;

		autoStrategyCompleted = autoController.Execute();

		std::cout << driveBase.GetLeftSpeed() << ' ' << driveBase.GetRightSpeed() << std::endl;

		if (autoStrategyCompleted)
		{
			std::cout << "Finished at " << autoTimer.Get() << " seconds" << std::endl;
			std::cout << "Left: " << std::setw(5) << driveBase.GetLeftDistance() << ' '
			          << "Right: " << std::setw(5) << driveBase.GetRightDistance() << ' '
					  << "Angle: " << std::setw(5) << gyroscope.GetAngle() << std::endl;
		}
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
	          << "Right: " << std::setw(5) << driveBase.GetRightDistance() << ' '
			  << "Angle: " << std::setw(5) << gyroscope.GetAngle() << std::endl;

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

	// Bulldozer
	if (perifController.GetRawButton(buttonBulldozerPulse) && !clamp.GetIsDown())
		bulldozerPulseToggle = true;

	if (perifController.GetRawButton(buttonBulldozerExtend) && !clamp.GetIsDown())
	{
		bulldozer.Extend();
		bulldozerPulseToggle = false;
	}
	else if (!bulldozerPulseToggle)
		bulldozer.Retract();

	if (bulldozerPulseToggle)
		bulldozerPulseToggle = !bulldozer.Pulse(0.5);

	// Kicker
	if (perifController.GetRawButton(buttonBulldozerKick))
		bulldozerKickToggle = true;

	if (bulldozerKickToggle)
		bulldozerKickToggle = !bulldozer.Kick(0.5);

	// Clamp
	if (perifController.GetRawButtonPressed(buttonArmToggle))
	{
		if (clamp.GetIsDown())
			clamp.ArmUp();
		else
			clamp.ArmDown();
	}

	if (perifController.GetRawButtonPressed(buttonClampToggle))
	{
		if (clamp.GetIsOpen())
			clamp.Close();
		else if (!bulldozer.GetIsExtended())
			clamp.Open();
	}
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

	driveBase.SetTrim(prefs->GetDouble("LeftForwardTrim", 1.0),
					  prefs->GetDouble("RightForwardTrim", 1.0),
					  prefs->GetDouble("LeftReverseTrim", 1.0),
					  prefs->GetDouble("RightReverseTrim", 1.0));

	speedNormal = prefs->GetFloat("SpeedNormal", 0.5f);
	speedTurtle = prefs->GetFloat("SpeedTurtle", 0.25f);
	speedBoost = prefs->GetFloat("SpeedBoost", 1.0f);

	// Get specified delay for autonomous
	frc::SmartDashboard::SetDefaultNumber("Auto Delay", 0);
	autoDelay = frc::SmartDashboard::GetNumber("Auto Delay", 0);

	// Setup autonomous strategy chooser
	autoStrategyChooser.AddObject("R Exchange", &AUTO_STRATEGIES::RIGHT_EXCHANGE_OPTIONS);
	autoStrategyChooser.AddObject("L Exchange", &AUTO_STRATEGIES::LEFT_EXCHANGE_OPTIONS);
	autoStrategyChooser.AddObject("R Inner Switch", &AUTO_STRATEGIES::RIGHT_SWITCH_INNER_OPTIONS);
	autoStrategyChooser.AddObject("L Inner Switch", &AUTO_STRATEGIES::LEFT_SWITCH_INNER_OPTIONS);
	autoStrategyChooser.AddObject("R Outer Switch", &AUTO_STRATEGIES::RIGHT_SWITCH_OUTER_OPTIONS);
	autoStrategyChooser.AddObject("L Outer Switch", &AUTO_STRATEGIES::LEFT_SWITCH_OUTER_OPTIONS);
	autoStrategyChooser.AddObject("Cross Line", &AUTO_STRATEGIES::CROSS_OPTIONS);
	autoStrategyChooser.AddObject("Cross Line Time", &AUTO_STRATEGIES::CROSS_TIME_OPTIONS);
	autoStrategyChooser.AddDefault("Do nothing", &AUTO_STRATEGIES::NOTHING_OPTIONS);
	autoStrategyChooser.AddObject("R Same Side Cross", &AUTO_STRATEGIES::CROSS_RIGHT_OPTIONS);
	autoStrategyChooser.AddObject("L Same Side Cross", &AUTO_STRATEGIES::CROSS_LEFT_OPTIONS);
	frc::SmartDashboard::PutData("Autonomous Strategies", &autoStrategyChooser);

	// Determine switch setup to select strategy.
	Position gameData[3];
	GetGameData(gameData);

	if (autoStrategyChooser.GetSelected())
	{
		if (gameData[0] == LEFT)
			selectedAutoStrategy = autoStrategyChooser.GetSelected()->leftOption;
		else
			selectedAutoStrategy = autoStrategyChooser.GetSelected()->rightOption;
	}
	else
	{
		selectedAutoStrategy = &AUTO_STRATEGIES::NOTHING;
		std::cout << "No strategy found" << std::endl;
	}

	// Backup strategy selection for older driver station
	SmartDashboard::SetDefaultString("AutoStrategyBackup", "nothing");

	if (selectedAutoStrategy == &AUTO_STRATEGIES::NOTHING)
	{
		std::string selectedAutoStrategyKey = SmartDashboard::GetString("AutoStrategyBackup", "nothing");

		if (selectedAutoStrategyKey.length() > 0)
		{
			const AutoController::PositionOptions* autoPosition = NULL;

			std::cout << "Using backup auto strategy " << selectedAutoStrategyKey << std::endl;

			if (selectedAutoStrategyKey == "nothing")
				autoPosition = &AUTO_STRATEGIES::NOTHING_OPTIONS;
			else if (selectedAutoStrategyKey == "cross")
				autoPosition = &AUTO_STRATEGIES::CROSS_OPTIONS;
			else if (selectedAutoStrategyKey == "leftswitch")
				autoPosition = &AUTO_STRATEGIES::LEFT_SWITCH_INNER_OPTIONS;
			else if (selectedAutoStrategyKey == "rightswitch")
				autoPosition = &AUTO_STRATEGIES::RIGHT_SWITCH_INNER_OPTIONS;
			else if (selectedAutoStrategyKey == "leftexchange")
				autoPosition = &AUTO_STRATEGIES::LEFT_EXCHANGE_OPTIONS;
			else if (selectedAutoStrategyKey == "rightexchange")
				autoPosition = &AUTO_STRATEGIES::RIGHT_EXCHANGE_OPTIONS;
			else
				std::cout << "No strategy exists with that name" << std::endl;

			if (autoPosition)
			{
				if (gameData[0] == LEFT)
					selectedAutoStrategy = autoPosition->leftOption;
				else
					selectedAutoStrategy = autoPosition->rightOption;
			}
		}
	}

	std::cout << "Updated Preferences" << std::endl;
}

START_ROBOT_CLASS(Robot)
