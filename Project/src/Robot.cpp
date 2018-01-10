#include "Timer.h"
#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
                 driveBase(0, 1), gripPipeline()
{
	speedNormal = 0.5f;
	speedTurtle = 0.25f;
	speedBoost = 1.0f;

	axisTankLeft = xbox::axis::leftY;
	axisTankRight = xbox::axis::rightY;
	buttonBoost = xbox::btn::lb;
	buttonTurtle = xbox::btn::rb;

	autoCrossTime = 1.0f;
	autoCrossSpeed = 0.5;
}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	cs::UsbCamera camera = CameraServer::GetInstance()->StartAutomaticCapture();
	camera.SetResolution(640, 480);
	camera.SetExposureManual(25);
	cvSink = CameraServer::GetInstance()->GetVideo();
}

void Robot::AutonomousInit()
{
	autoTimer.Reset();
	autoTimer.Start();
}

void Robot::AutonomousPeriodic()
{
	// This code should be multithreaded to increase efficiency
	cv::Mat image;
	cvSink.GrabFrame(image);
	gripPipeline.Process(image);
	// ================================

	if (autoTimer.Get() < autoCrossTime)
		driveBase.Drive(autoCrossSpeed);
	else
		driveBase.Stop();

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
