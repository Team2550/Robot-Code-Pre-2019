#include "Timer.h"
#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
                 choreographer(),
				 driveBase(),
				 shooter(),
				 lift()
{
	decreaseShooterSpeedDown = false;
	increaseShooterSpeedDown = false;
	climbToggleHold = false;
	climbToggle = false;
}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	SmartDashboard::PutNumber("Robot Speed (in/sec)", Autonomous::SpeedInchesPerSecond);
	SmartDashboard::PutNumber("Full Rotation Time", Autonomous::FullRotationTime);

	scenarioChooser.AddDefault("Far Left", &farLeftScenario);
	scenarioChooser.AddObject("Middle", &middleScenario);
	scenarioChooser.AddObject("Middle Right", &midRightScenario);
	scenarioChooser.AddObject("Far Right", &farRightScenario);
	SmartDashboard::PutData("Auto Scenario", &scenarioChooser);

	driveChooser.AddDefault("Normal", &normalDrive);
	driveChooser.AddObject("Backwards", &backwardsDrive);
	SmartDashboard::PutData("Drive Mode", &driveChooser);

	SmartDashboard::PutNumber("Left Forwards Ratio", Speeds::DriveBase::LeftPowerRatioForwards);
	SmartDashboard::PutNumber("Right Forwards Ratio", Speeds::DriveBase::RightPowerRatioForwards);
	SmartDashboard::PutNumber("Left Backwards Ratio", Speeds::DriveBase::LeftPowerRatioBackwards);
	SmartDashboard::PutNumber("Right Backwards Ratio", Speeds::DriveBase::RightPowerRatioBackwards);
}

void Robot::AutonomousInit()
{
	// Initialize choreographer to selected position scenario

	// Get scenario from smart dashboard
	Autonomous::PosScenario *autoPosScenario = scenarioChooser.GetSelected();
	// If not value could be retrieved, default to the value stored in the default scenario constant.
	// The pointers that autoPosScenario is being set to are the same that would have been retrieved by the function above.
	if(autoPosScenario == nullptr)
		switch (Autonomous::DefaultScenario)
		{
		case Autonomous::FarLeft:
			autoPosScenario = &farLeftScenario;
			break;
		case Autonomous::Middle:
			autoPosScenario = &middleScenario;
			break;
		case Autonomous::MidRight:
			autoPosScenario = &midRightScenario;
			break;
		case Autonomous::FarRight:
			autoPosScenario = &farRightScenario;
			break;
		}

	// Get dynamic fine-tuning values from smart dashboard
	float speedInchesPerSecond = SmartDashboard::GetNumber("Robot Speed (in/sec)", Autonomous::SpeedInchesPerSecond);
	float fullRotationTime = SmartDashboard::GetNumber("Full Rotation Time", Autonomous::FullRotationTime);

	// Set the timetable of the choreographer to the appropriate scenario
	// Currently, the lines that would be used in production are commented out
	// The functions being used are from the dynamic blind scenarios namespace.
	// They take the speed and rotation time values and use them to update the array passed to them with the appropriate timetable values
	//                                                                                                      (time, leftSpeed, rightSpeed)
	if (autoPosScenario == &farLeftScenario)
	{
		//choreographer.setTimetable(Autonomous::BlindScenarios::FarLeftPos::PeriodCount,
		//                           Autonomous::BlindScenarios::FarLeftPos::Timetable);
		float timetable[Autonomous::DynamicBlindScenarios::FarLeftPos::PeriodCount][3];
		Autonomous::DynamicBlindScenarios::FarLeftPos::timetable(speedInchesPerSecond, fullRotationTime, timetable);

		choreographer.setTimetable(Autonomous::DynamicBlindScenarios::MiddlePos::PeriodCount, timetable);
	}
	else if (autoPosScenario == &middleScenario)
	{
		//choreographer.setTimetable(Autonomous::BlindScenarios::MiddlePos::PeriodCount,
		//                           Autonomous::BlindScenarios::MiddlePos::Timetable);
		float timetable[Autonomous::DynamicBlindScenarios::MiddlePos::PeriodCount][3];
		Autonomous::DynamicBlindScenarios::MiddlePos::timetable(speedInchesPerSecond, fullRotationTime, timetable);

		choreographer.setTimetable(Autonomous::DynamicBlindScenarios::MiddlePos::PeriodCount, timetable);
	}
	else if (autoPosScenario == &midRightScenario)
	{
		//choreographer.setTimetable(Autonomous::BlindScenarios::MidRightPos::PeriodCount,
		//                           Autonomous::BlindScenarios::MidRightPos::Timetable);
		float timetable[Autonomous::DynamicBlindScenarios::MidRightPos::PeriodCount][3];
		Autonomous::DynamicBlindScenarios::MidRightPos::timetable(speedInchesPerSecond, fullRotationTime, timetable);

		choreographer.setTimetable(Autonomous::DynamicBlindScenarios::MiddlePos::PeriodCount, timetable);
	}
	else if (autoPosScenario == &farRightScenario)
	{
		//choreographer.setTimetable(Autonomous::BlindScenarios::FarRightPos::PeriodCount,
		//                           Autonomous::BlindScenarios::FarRightPos::Timetable);
		float timetable[Autonomous::DynamicBlindScenarios::FarRightPos::PeriodCount][3];
		Autonomous::DynamicBlindScenarios::FarRightPos::timetable(speedInchesPerSecond, fullRotationTime, timetable);

		choreographer.setTimetable(Autonomous::DynamicBlindScenarios::MiddlePos::PeriodCount, timetable);
	}

	driveBase.setReversed(false);

	autoTimer.Reset();
	autoTimer.Start();
}

void Robot::AutonomousPeriodic()
{
	choreographer.applyScheduleToRobot(autoTimer.Get(), driveBase);
}

void Robot::autoAim()
{
	printf("Aiming \n");

	float data[UDP::DataCount];
	udpReceiver.getUDPData(data);

	if (data[UDP::Index::Distance] > 5)
	{
		printf("Move forward");
	}
	else if (data[UDP::Index::Distance] < 5)
	{
		printf("Optimal distance");
	}
}

void Robot::TeleopInit()
{
	/* ========== DriveBase ========== */
	driveBase.stop();

	/* ========== Shooter ========== */
	blenderTimer.Stop();
	blenderTimer.Reset();
}

void Robot::TeleopPeriodic()
{
	/* ========== Settings ========== */
	DriveType *driveType = driveChooser.GetSelected();
	if(driveType == nullptr)
		driveBase.setReversed(false);
	else
		driveBase.setReversed(driveChooser.GetSelected() == &backwardsDrive);

	/* ========== udpReceiver ========== */
	udpReceiver.checkUDP();

	/* ========== DriveBase ========== */
	float leftSpeed = Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Left));
	float rightSpeed = Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Right));

	leftSpeed *= (leftSpeed > 0) ? SmartDashboard::GetNumber("Left Forwards Ratio", 1.0) :
	                               SmartDashboard::GetNumber("Left Backwards Ratio", 1.0);
	rightSpeed *= (rightSpeed > 0) ? SmartDashboard::GetNumber("Right Forwards Ratio", 1.0) :
	                                 SmartDashboard::GetNumber("Right Backwards Ratio", 1.0);

	bool boost = driveController.GetRawButton(Controls::TankDrive::Boost);
	bool turtle = driveController.GetRawButton(Controls::TankDrive::Turtle);
	float speed = turtle ? Speeds::DriveBase::Turtle : (boost ? Speeds::DriveBase::Boost : Speeds::DriveBase::Normal);
	driveBase.drive(leftSpeed *speed,
					rightSpeed * speed);

	/* ========== Shooter ========== */
	SmartDashboard::PutNumber("shooterCurrent", pdp.GetCurrent(Ports::PDP::Shooter));

	if (perifController.GetRawButton(Controls::Peripherals::Shoot))
	{
		shooter.shoot(pdp.GetCurrent(Ports::PDP::Shooter));

		blenderTimer.Start();
		//shooter.blend(fmod(timeSinceStart.Get(), 4) < 2.0);
		if(blenderTimer.Get() >= 4.0)
			shooter.blend(!perifController.GetRawButton(Controls::Peripherals::ReverseBlender));
	}
	else
	{
		shooter.stop();
		shooter.stopBlend();
		blenderTimer.Stop();
		blenderTimer.Reset();
	}

	if (perifController.GetRawButton(Controls::Peripherals::IncreaseShootSpeed))
	{
		if (!increaseShooterSpeedDown)
		{
			shooter.addSpeedOffset(0.01);
			printf("New shooter speed: ");
			printf(std::to_string(Speeds::Shooter::Shooter +
								  shooter.getSpeedOffset()).c_str());
			printf("\n");

			increaseShooterSpeedDown = true;
		}
	}
	else
		increaseShooterSpeedDown = false;

	if (perifController.GetRawButton(Controls::Peripherals::DecreaseShootSpeed))
	{
		if (!decreaseShooterSpeedDown)
		{
			shooter.addSpeedOffset(-0.01);
			printf("New shooter speed: ");
			printf(std::to_string(Speeds::Shooter::Shooter +
								  shooter.getSpeedOffset()).c_str());
			printf("\n");

			decreaseShooterSpeedDown = true;
		}
	}
	else
		decreaseShooterSpeedDown = false;

	/* ========== Lift/In-Feed ========== */
	if (perifController.GetRawButton(Controls::Peripherals::ClimbToggle))
	{
		if(climbToggleHold == false)
		{
			climbToggle = !climbToggle;
			climbToggleHold = true;
		}
	}
	else
		climbToggleHold = false;

	if(climbToggle || perifController.GetRawAxis(Controls::Peripherals::Climb) > 0.5)
		lift.raise();
	else
		lift.stop();
}

START_ROBOT_CLASS(Robot)
