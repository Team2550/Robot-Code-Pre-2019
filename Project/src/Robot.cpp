#include "Timer.h"
#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
                 choreographer(),
                 udpReceiver(),
				 driveBase(),
				 shooter(),
				 lift()
{
	decreaseShooterSpeedDown = false;
	increaseShooterSpeedDown = false;
	climbToggleHold = false;
	climbToggle = false;
	canAutoAim = false;
	autoSafeMode = true;
}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	scenarioChooser.AddDefault("Left", &leftScenario);
	scenarioChooser.AddObject("Middle", &middleScenario);
	scenarioChooser.AddObject("Right", &rightScenario);
	scenarioChooser.AddObject("Test Scenario", &testScenario);
	SmartDashboard::PutData("Auto Scenario", &scenarioChooser);

	SmartDashboard::SetDefaultNumber("Robot Speed Inches Per Second", Autonomous::SpeedInchesPerSecond);
	SmartDashboard::SetDefaultNumber("Full Rotation Time", Autonomous::FullRotationTime);

	// Default value to set smart dashboard item to is it's current value, unless it doesn't exist.
	SmartDashboard::SetDefaultNumber("Left Forwards Ratio", Speeds::DriveBase::LeftPowerRatioForwards);
	SmartDashboard::SetDefaultNumber("Right Forwards Ratio", Speeds::DriveBase::RightPowerRatioForwards);
	SmartDashboard::SetDefaultNumber("Left Backwards Ratio", Speeds::DriveBase::LeftPowerRatioBackwards);
	SmartDashboard::SetDefaultNumber("Right Backwards Ratio", Speeds::DriveBase::RightPowerRatioBackwards);

	SmartDashboard::SetDefaultBoolean("Camera Tracking", false);
	SmartDashboard::SetDefaultBoolean("Safe mode", true);
	SmartDashboard::SetDefaultNumber("Blind time multiplier", 1);
}

void Robot::AutonomousInit()
{
	// Initialize choreographer to selected position scenario
	// Get scenario from smart dashboard
	Autonomous::PosScenario *autoPosScenario = scenarioChooser.GetSelected();
	autoSafeMode = SmartDashboard::GetBoolean("Safe mode", true);
	printf(autoSafeMode ? "Safe mode\n" : "Not safe mode\n");

	//if value could not be retrieved, default to the value stored in the default scenario constant.
	// The pointers that autoPosScenario is being set to are the same that would have been retrieved by the function above.
	if(autoPosScenario == nullptr)
	{
		printf("No scenario found\n");
		switch (Autonomous::DefaultScenario)
		{
		case Autonomous::Left:
			autoPosScenario = &leftScenario;
			break;
		case Autonomous::Middle:
			autoPosScenario = &middleScenario;
			break;
		case Autonomous::Right:
			autoPosScenario = &rightScenario;
			break;
		case Autonomous::Test:
			autoPosScenario = &testScenario;
			break;
		}
	}

	// Get dynamic fine-tuning values from smart dashboard
	float blindTimeMultiplier = SmartDashboard::GetNumber("Blind time multiplier",1);

	// Set the timetable of the choreographer to the appropriate scenario
	// Currently, the lines that would be used in production are commented out
	// The functions being used are from the dynamic blind scenarios namespace.
	// They take the speed and rotation time values and use them to update the array passed to them with the appropriate timetable values
	//                                                                                                      (time, leftSpeed, rightSpeed)
	if (autoPosScenario == &leftScenario)
	{
		printf("Left\n");
		float timetable[Autonomous::DynamicBlindScenarios::LeftPos::PeriodCount][3];
		Autonomous::DynamicBlindScenarios::LeftPos::getTimetable(blindTimeMultiplier, timetable);

		choreographer.setTimetable(Autonomous::DynamicBlindScenarios::LeftPos::PeriodCount, timetable);
	}
	else if (autoPosScenario == &middleScenario)
	{
		printf("Middle\n");
		float timetable[Autonomous::DynamicBlindScenarios::MiddlePos::PeriodCount][3];
		Autonomous::DynamicBlindScenarios::MiddlePos::getTimetable(blindTimeMultiplier, timetable);

		choreographer.setTimetable(Autonomous::DynamicBlindScenarios::MiddlePos::PeriodCount, timetable);
	}
	else if (autoPosScenario == &rightScenario)
	{
		printf("Right\n");
		float timetable[Autonomous::DynamicBlindScenarios::RightPos::PeriodCount][3];
		Autonomous::DynamicBlindScenarios::RightPos::getTimetable(blindTimeMultiplier, timetable);

		choreographer.setTimetable(Autonomous::DynamicBlindScenarios::RightPos::PeriodCount, timetable);
	}
	else if (autoPosScenario == &testScenario)
	{
		printf("Test\n");
		float timetable[Autonomous::DynamicBlindScenarios::TestScenario::PeriodCount][3];
		Autonomous::DynamicBlindScenarios::TestScenario::getTimetable(blindTimeMultiplier, timetable);

		choreographer.setTimetable(Autonomous::DynamicBlindScenarios::TestScenario::PeriodCount, timetable);
	}

	canAutoAim = SmartDashboard::GetBoolean("Camera Tracking", false);

	driveBase.setReversed(true);

	autoTimer.Reset();
	autoTimer.Start();
}

void Robot::AutonomousPeriodic()
{
	/* ========== udpReceiver ========== */
	udpReceiver.checkUDP();

	float data[UDP::DataCount];
	udpReceiver.getUDPData(data);

	printf("X Offset:");
	printf(std::to_string(data[UDP::Index::XOffset]).c_str());
	printf(", Dist:");
	printf(std::to_string(data[UDP::Index::Distance]).c_str());
	printf("\n");

	/* ========== DriveBase ========== */
	// Run choreographer script until end of second to last step, unless can't auto aim
	if (autoSafeMode)
	{
		if (autoTimer.Get() < 4)
			driveBase.drive(Speeds::DriveBase::Turtle);
		else
			driveBase.stop();
	}
	else
	{
		if (!canAutoAim || autoTimer.Get() < choreographer.getPeriod(choreographer.getPeriodCount() - 1).time - 3.0)
		{
			choreographer.applyScheduleToRobot(autoTimer.Get(), driveBase);
		}
		else
		{
			autoAim();
		}
	}

	// Modify motor speeds based on trim from smartDashboard
	driveBase.applyTrim(SmartDashboard::GetNumber("Left Forwards Ratio", 1.0),
	                    SmartDashboard::GetNumber("Right Forwards Ratio", 1.0),
	                    SmartDashboard::GetNumber("Left Backwards Ratio", 1.0),
						SmartDashboard::GetNumber("Right Backwards Ratio", 1.0));
}

void Robot::TeleopInit()
{
	/* ========== DriveBase ========== */
	driveBase.setReversed(false);
	driveBase.stop();
}

void Robot::TeleopPeriodic()
{
	/* ========== udpReceiver ========== */
	udpReceiver.checkUDP();

	float data[UDP::DataCount];
	udpReceiver.getUDPData(data);

	printf("X Offset:");
	printf(std::to_string(data[UDP::Index::XOffset]).c_str());
	printf(", Dist:");
	printf(std::to_string(data[UDP::Index::Distance]).c_str());
	printf("\n");

	/* ========== DriveBase ========== */
	canAutoAim = SmartDashboard::GetBoolean("Camera Tracking", false);

	if (canAutoAim && driveController.GetRawButton(Controls::TankDrive::AutoAim))
	{
		autoAim();
	}
	else
	{
		float leftSpeed = Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Left));
		float rightSpeed = Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Right));

		bool boost = driveController.GetRawButton(Controls::TankDrive::Boost);
		bool turtle = driveController.GetRawButton(Controls::TankDrive::Turtle);

		float speed = turtle ? Speeds::DriveBase::Turtle : (boost ? Speeds::DriveBase::Boost : Speeds::DriveBase::Normal);

		driveBase.drive(leftSpeed * speed,
						rightSpeed * speed);
	}

	driveBase.applyTrim(SmartDashboard::GetNumber("Left Forwards Ratio", 1.0),
	                    SmartDashboard::GetNumber("Right Forwards Ratio", 1.0),
	                    SmartDashboard::GetNumber("Left Backwards Ratio", 1.0),
						SmartDashboard::GetNumber("Right Backwards Ratio", 1.0));

	/* ========== Shooter ========== */
	SmartDashboard::PutNumber("shooterCurrent", pdp.GetCurrent(Ports::PDP::Shooter));

	if (perifController.GetRawButton(Controls::Peripherals::Shoot))
		shooter.shoot(pdp.GetCurrent(Ports::PDP::Shooter));
	else
		shooter.stop();

	if(perifController.GetRawButton(Controls::Peripherals::Blender))
		shooter.blend(perifController.GetRawAxis(Controls::Peripherals::ReverseBlender) > 0.3);
	else
		shooter.stopBlend();

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

	if(climbToggle)
		lift.raise();
	else if(perifController.GetRawAxis(Controls::Peripherals::Climb) > 0.25)
		lift.raise(perifController.GetRawAxis(Controls::Peripherals::Climb));
	else
		lift.stop();

	/* ========== ========== */
	double amps = (pdp.GetCurrent(Ports::PDP::LeftMotor1) + pdp.GetCurrent(Ports::PDP::LeftMotor2) +
			       pdp.GetCurrent(Ports::PDP::RightMotor1) + pdp.GetCurrent(Ports::PDP::RightMotor2)) / 4;

	printf("Amps: ");
	printf(std::to_string(amps).c_str());
	printf("\n");
}

void Robot::autoAim()
{
	printf("Aiming...\n");

	float baseSpeed = driveBase.getReversed() ? Speeds::DriveBase::Turtle : -Speeds::DriveBase::Turtle;

	// Get data
	float data[UDP::DataCount];
	udpReceiver.getUDPData(data);

	if (udpReceiver.getUDPDataAge() > 1)
	{
		printf("Cannot see target! ");

		double amps = (pdp.GetCurrent(Ports::PDP::LeftMotor1) + pdp.GetCurrent(Ports::PDP::LeftMotor2) +
				       pdp.GetCurrent(Ports::PDP::RightMotor1) + pdp.GetCurrent(Ports::PDP::RightMotor2)) / 4;

		// Stop moving forward if motors are no longer spinning
		if (true)
		{
			printf("Moving forward...\n");
			driveBase.drive(baseSpeed * 0.75, baseSpeed * 0.75);
		}
		else
		{
			printf("Stopping...\n");
			driveBase.stop();
		}
	}
	else
	{
		if (data[UDP::Index::XOffset] > 10) // Max offset of 10, rotates in place
		{
			printf("Target is far left\n");

			driveBase.drive(baseSpeed * 1.5, -baseSpeed * 1.5);
		}
		else if (data[UDP::Index::XOffset] > 2) // Move while rotating
		{
			printf("Target is slight right\n");

			driveBase.drive(baseSpeed, 0);
		}
		else if (data[UDP::Index::XOffset] < -10)
		{
			printf("Target is far left\n");

			driveBase.drive(-baseSpeed * 1.5, baseSpeed * 1.5);
		}
		else if (data[UDP::Index::XOffset] < -2)
		{
			printf("Target is slight left\n");

			driveBase.drive(0, baseSpeed);
		}
		else if (data[UDP::Index::Distance] > 20)
		{
			printf("Target is distant\n");

			driveBase.drive(baseSpeed);
		}
		else if (data[UDP::Index::Distance] > 7.5f)
		{
			printf("Target is near\n");

			driveBase.drive(baseSpeed * 0.75);
		}
		else
		{
			printf("At target\n");
			driveBase.stop();
		}
	}
}

START_ROBOT_CLASS(Robot)
