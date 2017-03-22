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
	autoScenarioChooser.AddDefault("Left", &leftScenario);
	autoScenarioChooser.AddObject("Middle", &middleScenario);
	autoScenarioChooser.AddObject("Right", &rightScenario);
	autoScenarioChooser.AddObject("Test Scenario", &testScenario);
	SmartDashboard::PutData("Auto Scenario", &autoScenarioChooser);

	// Auto speeds
	SmartDashboard::SetDefaultNumber("Robot Speed Inches Per Second", Autonomous::SpeedInchesPerSecond);
	SmartDashboard::SetDefaultNumber("Full Rotation Time", Autonomous::FullRotationTime);

	SmartDashboard::SetPersistent("Robot Speed Inches Per Second");
	SmartDashboard::SetPersistent("Full Rotation Time");

	// Motor trim
	SmartDashboard::SetDefaultNumber("Left Forwards Ratio", Speeds::DriveBase::LeftPowerRatioForwards);
	SmartDashboard::SetDefaultNumber("Right Forwards Ratio", Speeds::DriveBase::RightPowerRatioForwards);
	SmartDashboard::SetDefaultNumber("Left Backwards Ratio", Speeds::DriveBase::LeftPowerRatioBackwards);
	SmartDashboard::SetDefaultNumber("Right Backwards Ratio", Speeds::DriveBase::RightPowerRatioBackwards);

	SmartDashboard::SetPersistent("Left Forwards Ratio");
	SmartDashboard::SetPersistent("Right Forwards Ratio");
	SmartDashboard::SetPersistent("Left Backwards Ratio");
	SmartDashboard::SetPersistent("Right Backwards Ratio");

	// Safety settings
	SmartDashboard::SetDefaultBoolean("Camera Tracking", false);
	SmartDashboard::SetDefaultBoolean("Safe mode", true);
	SmartDashboard::SetDefaultBoolean("Rumble Active", false);
	SmartDashboard::SetDefaultNumber("Blind time multiplier", 1);

	SmartDashboard::SetPersistent("Camera Tracking");
	SmartDashboard::SetPersistent("Safe mode");
	SmartDashboard::SetPersistent("Blind time multiplier");
}

void Robot::AutonomousInit()
{
	// Initialize choreographer to selected position scenario
	// Get scenario from smart dashboard
	Autonomous::PosScenario *autoPosScenario = autoScenarioChooser.GetSelected();
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
		printf("Pos Left\n");
		float timetable[Autonomous::DynamicBlindScenarios::LeftPos::PeriodCount][3];
		Autonomous::DynamicBlindScenarios::LeftPos::getTimetable(blindTimeMultiplier, timetable);

		choreographer.setTimetable(Autonomous::DynamicBlindScenarios::LeftPos::PeriodCount, timetable);
	}
	else if (autoPosScenario == &middleScenario)
	{
		printf("Pos Middle\n");
		float timetable[Autonomous::DynamicBlindScenarios::MiddlePos::PeriodCount][3];
		Autonomous::DynamicBlindScenarios::MiddlePos::getTimetable(blindTimeMultiplier, timetable);

		choreographer.setTimetable(Autonomous::DynamicBlindScenarios::MiddlePos::PeriodCount, timetable);
	}
	else if (autoPosScenario == &rightScenario)
	{
		printf("Pos Right\n");
		float timetable[Autonomous::DynamicBlindScenarios::RightPos::PeriodCount][3];
		Autonomous::DynamicBlindScenarios::RightPos::getTimetable(blindTimeMultiplier, timetable);

		choreographer.setTimetable(Autonomous::DynamicBlindScenarios::RightPos::PeriodCount, timetable);
	}
	else if (autoPosScenario == &testScenario)
	{
		printf("Test Mode \n");
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

	printf("X Angle:");
	printf(std::to_string(data[UDP::Index::HorizAngle]).c_str());
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

	printf("X Angle:");
	printf(std::to_string(data[UDP::Index::HorizAngle]).c_str());
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

	printf("Set motors: left = ");
	printf(std::to_string(driveBase.getLeftSpeed()).c_str());
	printf(", right = ");
	printf(std::to_string(driveBase.getRightSpeed()).c_str());
	printf("\n");

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

	/* ========== Amps feedback ========== */
	double amps = driveBase.getAmps(pdp);

	printf("Amps: ");
	printf(std::to_string(amps).c_str());
	printf("\n");

	/* ============ Rumble Feedback =========== */
	bool doRumble = SmartDashboard::GetBoolean("Rumble Active", false);

	if (doRumble && canAutoAim)
	{
		float data[UDP::DataCount];
		udpReceiver.getUDPData(data);

		float vibrationLevel = data[UDP::Index::XOffset] * .05;

		vibrationLevel = fmin(1, fmax(-1, vibrationLevel));

		Utility::setRumble(driveController, Utility::RumbleSide::both, 0);
		if (vibrationLevel < -0.1)
			Utility::setRumble(driveController, Utility::RumbleSide::left, -vibrationLevel);
		else if (vibrationLevel > 0.1)
			Utility::setRumble(driveController, Utility::RumbleSide::right, vibrationLevel);
	}
}

void Robot::autoAim()
{
	// Backup value of reversal to restore when finished. Set motor reversal to true (front = camera)
	bool wasReversed = driveBase.getReversed();
	driveBase.setReversed(true);

	// Get amps for checking if against wall
	double amps = driveBase.getAmps(pdp);

	// Initialize base speed
	float baseSpeed = 0.3;

	// Get vision data
	float data[UDP::DataCount];
	udpReceiver.getUDPData(data);

	printf("Aiming...\n");

	// Stop moving forward if motors are no longer spinning (amp limit = 20)
	if (amps > 20)
	{
		printf("Amps too high! Stopping...\n");
		driveBase.stop();
	}
	else
	{
		if (!udpReceiver.getUDPDataIsReal() || udpReceiver.getUDPDataAge() > 2)
		{
			printf("Cannot see target! ");

			// If data is not real, than target was never seen. Move forward blindly.
			if (!udpReceiver.getUDPDataIsReal())
			{
				printf("Target never seen, moving forward...\n");

				driveBase.drive(baseSpeed * 0.75);
			}
			// If xOffset is greater than 15, than target was last seen to the right. Rotate right.
			else if (data[UDP::Index::XOffset] > 15)
			{
				printf("Target last seen on right, rotating right. \n");

				driveBase.drive(baseSpeed * 0.75, -baseSpeed * 0.75);
			}
			// If xOffset is less than -15, than target was last seen to the left. Rotate left.
			else if (data[UDP::Index::XOffset] < -15)
			{
				printf("Target last seen on left, rotating left. \n");

				driveBase.drive(-baseSpeed * 0.75, baseSpeed * 0.75);
			}
			// If nothing else applies, than target was last seen in front. Move forward.
			else
			{
				printf("Target last seen centered, moving forward...\n");

				driveBase.drive(baseSpeed * 0.75);
			}

		}
		else
		{
			// Target is more than 15 degrees to the right. Rotate right.
			if (data[UDP::Index::HorizAngle] > 15)
			{
				printf("Target is far right\n");

				driveBase.drive(baseSpeed, -baseSpeed);
			}
			// Target is more than 5 degrees to the right. Rotate right and move forward.
			else if (data[UDP::Index::HorizAngle] > 5) // Move while rotating
			{
				printf("Target is slight right\n");

				driveBase.drive(baseSpeed, baseSpeed * 0.25);
			}
			// Target is more than 15 degrees to the left. Rotate left.
			else if (data[UDP::Index::HorizAngle] < -15)
			{
				printf("Target is far left\n");

				driveBase.drive(-baseSpeed, baseSpeed);
			}
			// Target is more than 5 degrees to the left. Rotate left and move forward.
			else if (data[UDP::Index::HorizAngle] < -5)
			{
				printf("Target is slight left\n");

				driveBase.drive(baseSpeed * 0.25, baseSpeed);
			}
			// Target is about centered but is distant. Move forward.
			else if (data[UDP::Index::Distance] > 30)
			{
				printf("Target is distant\n");

				driveBase.drive(baseSpeed);
			}
			// Target is about centered and close. Move forward slowly.
			else if (data[UDP::Index::Distance] > 7)
			{
				printf("Target is near\n");

				driveBase.drive(baseSpeed * 0.8);
			}
			// Robot is at target. Stop.
			else
			{
				printf("At target\n");

				driveBase.stop();
			}
		}
	}

	driveBase.setReversed(wasReversed);
}

START_ROBOT_CLASS(Robot)
