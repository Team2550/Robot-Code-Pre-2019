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
}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	SmartDashboard::PutNumber("Robot Speed Inches Per Second", Autonomous::SpeedInchesPerSecond);
	SmartDashboard::PutNumber("Full Rotation Time", Autonomous::FullRotationTime);

	scenarioChooser.AddDefault("Far Left", &farLeftScenario);
	scenarioChooser.AddObject("Middle", &middleScenario);
	scenarioChooser.AddObject("Middle Right", &midRightScenario);
	scenarioChooser.AddObject("Far Right", &farRightScenario);
	scenarioChooser.AddObject("Test Scenario", &testScenario);
	SmartDashboard::PutData("Auto Scenario", &scenarioChooser);

	safemodeSwitch.AddDefault("Off", &off);
	safemodeSwitch.AddObject("On", &on);
	SmartDashboard::PutData("Safemode", &safemodeSwitch);


	driveChooser.AddDefault("Normal", &normalDrive);
	driveChooser.AddObject("Backwards", &backwardsDrive);
	SmartDashboard::PutData("Drive Mode", &driveChooser);

	SmartDashboard::PutNumber("Left Forwards Ratio", Speeds::DriveBase::LeftPowerRatioForwards);
	SmartDashboard::PutNumber("Right Forwards Ratio", Speeds::DriveBase::RightPowerRatioForwards);
	SmartDashboard::PutNumber("Left Backwards Ratio", Speeds::DriveBase::LeftPowerRatioBackwards);
	SmartDashboard::PutNumber("Right Backwards Ratio", Speeds::DriveBase::RightPowerRatioBackwards);

	SmartDashboard::PutBoolean("Is camera tracking ready", false);
}

void Robot::AutonomousInit()
{
	// Initialize choreographer to selected position scenario

	// Get scenario from smart dashboard
	Autonomous::PosScenario *autoPosScenario = scenarioChooser.GetSelected();
	Autonomous::safeMode *autoSafeMode = safemodeSwitch.GetSelected();
	//if value could not be retrieved, default to the value stored in the default scenario constant.
	// The pointers that autoPosScenario is being set to are the same that would have been retrieved by the function above.
	if(autoPosScenario == nullptr)
	{
		printf("No scenario found\n");
		switch (Autonomous::DefaultScenario)
		{
		case Autonomous::FarLeft:
			if(autoSafeMode == &on){
				autoPosScenario = &safeFarLeftScenario;			}
			else{
				autoPosScenario = &farLeftScenario;
				break;
			}
		case Autonomous::Middle:
			if(autoSafeMode == &on){
				autoPosScenario = &safeMiddleScenario;
			}
			else{
				autoPosScenario = &middleScenario;
				break;
			}
		case Autonomous::MidRight:
			if(autoSafeMode == &on){
				autoPosScenario = &safeMidRightScenario;
			}
			else{
			autoPosScenario = &midRightScenario;
			break;
			}
		case Autonomous::FarRight:
			autoPosScenario = &farRightScenario;
			break;
		case Autonomous::Test:
			autoPosScenario = &testScenario;
			break;
		}
	}

	// Get dynamic fine-tuning values from smart dashboard
	float speedInchesPerSecond = SmartDashboard::GetNumber("Robot Speed Inches Per Second", Autonomous::SpeedInchesPerSecond);
	float fullRotationTime = SmartDashboard::GetNumber("Full Rotation Time", Autonomous::FullRotationTime);

	// Set the timetable of the choreographer to the appropriate scenario
	// Currently, the lines that would be used in production are commented out
	// The functions being used are from the dynamic blind scenarios namespace.
	// They take the speed and rotation time values and use them to update the array passed to them with the appropriate timetable values
	//                                                                                                      (time, leftSpeed, rightSpeed)
	if (autoPosScenario == &farLeftScenario)
	{
		printf("Far Left\n");
		//choreographer.setTimetable(Autonomous::BlindScenarios::FarLeftPos::PeriodCount,
		//                           Autonomous::BlindScenarios::FarLeftPos::Timetable);
		float timetable[Autonomous::DynamicBlindScenarios::FarLeftPos::PeriodCount][3];
		Autonomous::DynamicBlindScenarios::FarLeftPos::getTimetable(speedInchesPerSecond, fullRotationTime, timetable);

		choreographer.setTimetable(Autonomous::DynamicBlindScenarios::FarLeftPos::PeriodCount, timetable);
	}
	else if (autoPosScenario == &middleScenario)
	{
		printf("Middle\n");
		//choreographer.setTimetable(Autonomous::BlindScenarios::MiddlePos::PeriodCount,
		//                           Autonomous::BlindScenarios::MiddlePos::Timetable);
		float timetable[Autonomous::DynamicBlindScenarios::MiddlePos::PeriodCount][3];
		Autonomous::DynamicBlindScenarios::MiddlePos::getTimetable(speedInchesPerSecond, fullRotationTime, timetable);

		choreographer.setTimetable(Autonomous::DynamicBlindScenarios::MiddlePos::PeriodCount, timetable);
	}
	else if (autoPosScenario == &midRightScenario)
	{
		printf("Mid Right\n");
		//choreographer.setTimetable(Autonomous::BlindScenarios::MidRightPos::PeriodCount,
		//                           Autonomous::BlindScenarios::MidRightPos::Timetable);
		float timetable[Autonomous::DynamicBlindScenarios::MidRightPos::PeriodCount][3];
		Autonomous::DynamicBlindScenarios::MidRightPos::getTimetable(speedInchesPerSecond, fullRotationTime, timetable);

		choreographer.setTimetable(Autonomous::DynamicBlindScenarios::MidRightPos::PeriodCount, timetable);
	}
	else if (autoPosScenario == &farRightScenario)
	{
		printf("Far Right\n");
		//choreographer.setTimetable(Autonomous::BlindScenarios::FarRightPos::PeriodCount,
		//                           Autonomous::BlindScenarios::FarRightPos::Timetable);
		float timetable[Autonomous::DynamicBlindScenarios::FarRightPos::PeriodCount][3];
		Autonomous::DynamicBlindScenarios::FarRightPos::getTimetable(speedInchesPerSecond, fullRotationTime, timetable);

		choreographer.setTimetable(Autonomous::DynamicBlindScenarios::FarRightPos::PeriodCount, timetable);
	}
	else if (autoPosScenario == &testScenario)
	{
		printf("Test\n");
		//choreographer.setTimetable(Autonomous::BlindScenarios::FarRightPos::PeriodCount,
		//                           Autonomous::BlindScenarios::FarRightPos::Timetable);
		float timetable[Autonomous::DynamicBlindScenarios::TestScenario::PeriodCount][3];
		Autonomous::DynamicBlindScenarios::TestScenario::getTimetable(speedInchesPerSecond, fullRotationTime, timetable);

		choreographer.setTimetable(Autonomous::DynamicBlindScenarios::TestScenario::PeriodCount, timetable);
	}

	canAutoAim = SmartDashboard::GetBoolean("Is camera tracking ready", false);

	driveBase.setReversed(false);

	autoTimer.Reset();
	autoTimer.Start();
}

void Robot::AutonomousPeriodic()
{
	/* ========== udpReceiver ========== */
	udpReceiver.checkUDP();

	/* ========== DriveBase ========== */
	// Run choreographer script until end of second to last step, unless can't auto aim
	if (!canAutoAim || autoTimer.Get() < choreographer.getPeriod(choreographer.getPeriodCount() - 2).time)
	{
		choreographer.applyScheduleToRobot(autoTimer.Get(), driveBase);

		// Modify motor speeds based on trim from smartDashboard
		float leftSpeed = driveBase.getLeftSpeed();
		float rightSpeed = driveBase.getRightSpeed();

		leftSpeed *= (leftSpeed > 0) ? SmartDashboard::GetNumber("Left Forwards Ratio", 1.0) :
									   SmartDashboard::GetNumber("Left Backwards Ratio", 1.0);
		rightSpeed *= (rightSpeed > 0) ? SmartDashboard::GetNumber("Right Forwards Ratio", 1.0) :
										 SmartDashboard::GetNumber("Right Backwards Ratio", 1.0);

		driveBase.drive(leftSpeed, rightSpeed);
	}
	else
	{
		autoAim();
	}
}

void Robot::autoAim()
{
	printf("Aiming...\n");

	// Get data
	float data[UDP::DataCount];
	udpReceiver.getUDPData(data);

	if (udpReceiver.getUDPDataAge() > 1.5)
	{
		printf("Cannot see target! ");
		if (!udpReceiver.getUDPDataIsReal() || data[UDP::Index::Distance] > 15)
		{
			printf("Rotating...\n");
			driveBase.drive(0.4, -0.4);
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
			driveBase.drive(0.4, -0.4);
		}
		else if (data[UDP::Index::XOffset] > 2) // Move while rotating
		{
			printf("Target is slight right\n");
			driveBase.drive(0.4, 0);
		}
		else if (data[UDP::Index::XOffset] < -10)
		{
			printf("Target is far left\n");
			driveBase.drive(-0.4, 0.4);
		}
		else if (data[UDP::Index::XOffset] < -2)
		{
			printf("Target is slight left\n");
			driveBase.drive(0, 0.4);
		}
		else if (data[UDP::Index::Distance] > 20)
		{
			printf("Target is distant\n");
			driveBase.drive(0.6);
		}
		else if (data[UDP::Index::Distance] > 10)
		{
			printf("Target is near\n");
			driveBase.drive(0.3);
		}
		else
		{
			printf("At target\n");
			driveBase.stop();
		}
	}
}


void Robot::TeleopInit()
{
	/* ========== DriveBase ========== */
	driveBase.stop();
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
		shooter.shoot(pdp.GetCurrent(Ports::PDP::Shooter));
	else
		shooter.stop();

	if(perifController.GetRawButton(Controls::Peripherals::Blender))
		shooter.blend(!perifController.GetRawButton(Controls::Peripherals::ReverseBlender));
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

	if(perifController.GetRawAxis(Controls::Peripherals::Climb) > 0.25)
		lift.raise(perifController.GetRawAxis(Controls::Peripherals::Climb));
	else
		lift.stop();
}

START_ROBOT_CLASS(Robot)
