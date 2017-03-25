#include "Timer.h"
#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
                 udpReceiver(),
				 driveBase(),
				 shooter(),
				 lift()
{
	decreaseShooterSpeedDown = false;
	increaseShooterSpeedDown = false;
	climbToggleHold = false;
	climbToggle = false;
	autoReady = &safeReady;
	autoScenario = &middleScenario;
	targetRumble = &targetRumbleOff;
}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	clearSmartDashboard();

	// Autonomous readiness
	autoReadyChooser.AddDefault("Go past baseline", &safeReady);
	autoReadyChooser.AddObject("Place gear blind", &blindReady);
	autoReadyChooser.AddObject("Place gear vision", &visionReady);
	SmartDashboard::PutData("Autonomous Readiness", &autoReadyChooser);

	SmartDashboard::SetPersistent("Autonomous Readiness");

	// Autonomous position
	autoScenarioChooser.AddDefault("Middle", &middleScenario);
	autoScenarioChooser.AddObject("Side", &sideScenario);
	SmartDashboard::PutData("Auto Scenario", &autoScenarioChooser);

	// Targeting Rumble Feedback
	targetRumbleChooser.AddDefault("Off", &targetRumbleOff);
	targetRumbleChooser.AddObject("On", &targetRumbleOn);
	SmartDashboard::PutData("Targeting Rumble Feedback", &targetRumbleChooser);

	SmartDashboard::SetPersistent("Autonomous Readiness");

	// Motor trim
	SmartDashboard::SetDefaultNumber("Left Forwards Ratio", Speeds::DriveBase::LeftPowerRatioForwards);
	SmartDashboard::SetDefaultNumber("Right Forwards Ratio", Speeds::DriveBase::RightPowerRatioForwards);
	SmartDashboard::SetDefaultNumber("Left Backwards Ratio", Speeds::DriveBase::LeftPowerRatioBackwards);
	SmartDashboard::SetDefaultNumber("Right Backwards Ratio", Speeds::DriveBase::RightPowerRatioBackwards);

	SmartDashboard::SetPersistent("Left Forwards Ratio");
	SmartDashboard::SetPersistent("Right Forwards Ratio");
	SmartDashboard::SetPersistent("Left Backwards Ratio");
	SmartDashboard::SetPersistent("Right Backwards Ratio");
}

void Robot::AutonomousInit()
{
	// Get auto ready from smart dashboard.
	autoReady = autoReadyChooser.GetSelected();

	printf(autoReady == &safeReady ? "Safe mode\n" : "Not safe mode\n");

	if (autoReady != &safeReady)
	{
		// Get auto scenario from smart dashboard.
		autoScenario = autoScenarioChooser.GetSelected();

		// If value could not be retrieved, default to the value stored in the default scenario constant.
		// The pointers that autoPosScenario is being set to are the same that would have been retrieved by the function above.
		if(autoScenario == nullptr)
		{
			printf("No scenario found\n");
			switch (Autonomous::DefaultScenario)
			{
			case Autonomous::Middle:
				autoScenario = &middleScenario;
				break;
			case Autonomous::Side:
				autoScenario = &sideScenario;
				break;
			}
		}
	}

	// Set motor reversal and reset timer
	driveBase.setReversed(true);

	autoTimer.Reset();
	autoTimer.Start();
}

void Robot::AutonomousPeriodic()
{
	/* ========== udpReceiver ========== */
	udpReceiver.checkUDP();
	udpReceiver.clearUDPSocket();

	float data[UDP::DataCount];
	udpReceiver.getUDPData(data);

	printf("X Angle:");
	printf(std::to_string(data[UDP::Index::HorizAngle]).c_str());
	printf(", Dist:");
	printf(std::to_string(data[UDP::Index::Distance]).c_str());
	printf("\n");

	/* ========== DriveBase ========== */

	float blindTime = 6;
	float blindSpeed = Speeds::DriveBase::Turtle;

	if (autoReady != &safeReady)
	{
		if (autoScenario == &middleScenario)
		{
			blindTime = Autonomous::BlindTimes::Middle;
			blindSpeed = Autonomous::BlindSpeeds::Middle;
		}
		else if (autoScenario == &sideScenario)
		{
			blindTime = Autonomous::BlindTimes::Side;
			blindSpeed = Autonomous::BlindSpeeds::Side;
		}
	}

	// Run choreographer script until end of second to last step, unless can't auto aim
	if (autoReady != &visionReady || autoTimer.Get() < blindTime - 2.5)
	{
		if (autoTimer.Get() < blindTime)
			driveBase.drive(blindSpeed);
		else if (autoTimer.Get() < blindTime + 0.1)
			driveBase.drive(-blindSpeed * 0.75);
		else
			driveBase.stop();
	}
	else
	{
		autoAim();
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
	autoReady = autoReadyChooser.GetSelected();

	// Get rumble settings
	targetRumble = targetRumbleChooser.GetSelected();

	if(autoScenario == nullptr)
		targetRumble = &targetRumbleOff;

	/* ========== udpReceiver ========== */
	udpReceiver.checkUDP();
	udpReceiver.clearUDPSocket();

	float data[UDP::DataCount];
	udpReceiver.getUDPData(data);

	printf("X Angle:");
	printf(std::to_string(data[UDP::Index::HorizAngle]).c_str());
	printf(", Dist:");
	printf(std::to_string(data[UDP::Index::Distance]).c_str());
	printf("\n");


	/* ========== DriveBase ========== */
	if (autoReady == &visionReady && driveController.GetRawButton(Controls::TankDrive::AutoAim))
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
	//SmartDashboard::PutNumber("shooterCurrent", shooter.getAmps(pdp));

	if (perifController.GetRawButton(Controls::Peripherals::Shoot))
		shooter.shoot(shooter.getAmps(pdp));
	else
		shooter.stop();

	if (perifController.GetRawButton(Controls::Peripherals::Blender))
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

	/* ========== Amps Feedback ========== */
	//double amps = driveBase.getAmps(pdp);

	//printf("Amps: ");
	//printf(std::to_string(amps).c_str());
	//printf("\n");

	/* ============ Rumble Feedback =========== */
	Utility::setRumble(driveController, Utility::RumbleSide::both, 0);

	if (targetRumble == &targetRumbleOn && autoReady == &visionReady && udpReceiver.getUDPDataIsReal() && udpReceiver.getUDPDataAge() < 1)
	{
		float vibrationLevel = data[UDP::Index::HorizAngle] / 35.0f;

		vibrationLevel = fmin(0.8, fmax(-0.8, vibrationLevel));

		if (vibrationLevel < -0.1)
			Utility::setRumble(driveController, Utility::RumbleSide::left, -vibrationLevel);
		else if (vibrationLevel > 0.1)
			Utility::setRumble(driveController, Utility::RumbleSide::right, vibrationLevel);
	}
}

void Robot::DisabledInit()
{
	Utility::setRumble(driveController, Utility::RumbleSide::both, 0);
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

	printf("X Angle: ");
	printf(std::to_string(data[UDP::Index::HorizAngle]).c_str());
	printf(", Dist: ");
	printf(std::to_string(data[UDP::Index::Distance]).c_str());
	printf("\n");

	// Stop moving forward if motors are no longer spinning
	if (amps > Autonomous::AmpLimit)
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

				driveBase.drive(baseSpeed * 0.8);
			}
			// If xOffset is greater than 15, than target was last seen to the right. Rotate right.
			else if (data[UDP::Index::HorizAngle] > 10)
			{
				printf("Target last seen on right, rotating right. \n");

				driveBase.drive(baseSpeed, -baseSpeed);
			}
			// If xOffset is less than -15, than target was last seen to the left. Rotate left.
			else if (data[UDP::Index::HorizAngle] < -10)
			{
				printf("Target last seen on left, rotating left. \n");

				driveBase.drive(-baseSpeed, baseSpeed);
			}
			// If nothing else applies, than target was last seen in front. Move forward.
			else
			{
				printf("Target last seen centered, moving forward...\n");

				driveBase.drive(baseSpeed * 0.8);
			}

		}
		else
		{
			// Target is more than 15 degrees to the right. Rotate right.
			if (data[UDP::Index::HorizAngle] > 10)
			{
				printf("Target is far right\n");

				driveBase.drive(baseSpeed, -baseSpeed);
			}
			// Target is more than 5 degrees to the right. Rotate right and move forward.
			else if (data[UDP::Index::HorizAngle] > 3) // Move while rotating
			{
				printf("Target is slight right\n");

				driveBase.drive(baseSpeed, baseSpeed * 0.5);
			}
			// Target is more than 15 degrees to the left. Rotate left.
			else if (data[UDP::Index::HorizAngle] < -7)
			{
				printf("Target is far left\n");

				driveBase.drive(-baseSpeed, baseSpeed);
			}
			// Target is more than 5 degrees to the left. Rotate left and move forward.
			else if (data[UDP::Index::HorizAngle] < -3)
			{
				printf("Target is slight left\n");

				driveBase.drive(baseSpeed * 0.5, baseSpeed);
			}
			// Target is about centered but is distant. Move forward.
			else if (data[UDP::Index::Distance] > 7)
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

void Robot::clearSmartDashboard()
{
	SmartDashboard::ClearPersistent("Robot Speed Inches Per Second");
	SmartDashboard::ClearPersistent("Camera Tracking");
	SmartDashboard::ClearPersistent("Blind time multiplier");
	SmartDashboard::ClearPersistent("Safe mode");
	SmartDashboard::ClearPersistent("Full Rotation Time");
	SmartDashboard::ClearPersistent("Rumble Active");
}

START_ROBOT_CLASS(Robot)
