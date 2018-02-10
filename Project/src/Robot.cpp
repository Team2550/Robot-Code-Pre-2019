#include "Timer.h"
#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
                 ultrasonic(0, (5 / 4.88) * (1000 / 25.4)), // (5 mm / 4.88 mV) * (1/25.4 in/mm) * (1000 mV/V) Makes distance measure in INCHES
				 gyroscope(frc::SPI::Port::kOnboardCS0),
				 driveBase(0, 1, 0, 1, 2, 3, (6 * M_PI) / 360) // (circumference / 360 pulses per rotation)
{
	speedNormal = 0.5f;
	speedTurtle = 0.25f;
	speedBoost = 1.0f;

	axisTankLeft = xbox::axis::leftY;
	axisTankRight = xbox::axis::rightY;
	buttonBoost = xbox::btn::lb;
	buttonTurtle = xbox::btn::rb;

	prefs = Preferences::GetInstance();
}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	UpdatePreferences();
	//chooser.AddDefault("Default Auto", new );
	//chooser.AddObject("My Auto", new );
	frc::SmartDashboard::PutData("Auto Modes", &chooser);

	gyroscope.Calibrate();
}

void Robot::AutonomousInit()
{
	autonomousCommand.reset(chooser.GetSelected());
	if(autonomousCommand.get() != nullptr)
	{
		autonomousCommand->Start();
	}
	UpdatePreferences();

	gyroscope.Reset();
	driveBase.ResetDistance();

	autoTimer.Reset();
	autoTimer.Start();
}

void Robot::AutonomousPeriodic()
{
	std::cout << "Left: " << std::setw(5) << driveBase.GetLeftDistance() << ' '
	          << "Right: " << std::setw(5) << driveBase.GetRightDistance() << std::endl;

	double leftDistInches = driveBase.GetLeftDistance();
	double rightDistInches = driveBase.GetRightDistance();

	double avgDistInches = (leftDistInches + rightDistInches) / 2;

	float speed = 0;

	// Drive 36 inches at turtle speed. Then set speed to 0.
	if (avgDistInches < 36)
		speed = speedTurtle;
	else
		speed = 0;

	std::cout << gyroscope.GetAngle() << std::endl;

	// Offsets left and right speeds if one side drifts too far.
	if (gyroscope.GetAngle() < 1)
		driveBase.Drive(speed + 0.1, speed - 0.1);
	else if (gyroscope.GetAngle() > 1)
		driveBase.Drive(speed - 0.1, speed + 0.1);
	else
		driveBase.Drive(speed);

	frc::Scheduler::GetInstance()->Run();
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

	float leftSpeed = Utility::Deadzone(-driveController.GetRawAxis(axisTankLeft));
	float rightSpeed = Utility::Deadzone(-driveController.GetRawAxis(axisTankRight));

	float baseSpeed = speedNormal;

	if (driveController.GetRawButton(buttonTurtle))
		baseSpeed = speedTurtle;
	else if (driveController.GetRawButton(buttonBoost))
		baseSpeed = speedBoost;

	std::cout << gyroscope.GetAngle() << std::endl;

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
}

START_ROBOT_CLASS(Robot)
