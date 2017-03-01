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
	Autonomous::PosScenario *autoPosScenario = scenarioChooser.GetSelected();
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

	if (autoPosScenario == &farLeftScenario)
		choreographer.setTimetable(Autonomous::BlindScenarioFarLeftPos::PeriodCount,
		                           Autonomous::BlindScenarioFarLeftPos::Timetable);
	else if (autoPosScenario == &middleScenario)
		choreographer.setTimetable(Autonomous::BlindScenarioMiddlePos::PeriodCount,
		                           Autonomous::BlindScenarioMiddlePos::Timetable);
	else if (autoPosScenario == &midRightScenario)
		choreographer.setTimetable(Autonomous::BlindScenarioMidRightPos::PeriodCount,
		                           Autonomous::BlindScenarioMidRightPos::Timetable);
	else if (autoPosScenario == &farRightScenario)
		choreographer.setTimetable(Autonomous::BlindScenarioFarRightPos::PeriodCount,
		                           Autonomous::BlindScenarioFarRightPos::Timetable);

	driveBase.setReversed(false);

	autoTimer.Reset();
	autoTimer.Start();
}

void Robot::AutonomousPeriodic()
{
	choreographer.applyScheduleToRobot(autoTimer.Get(), driveBase);
}

//The scenarios are organized from 1 being the farthest left section against the wall and 4 being the farthest right section
/*void Robot::scenario1()
{
	printf("senario1 \n");
}
void Robot::scenario2()
{
	printf("scenario2 \n");
	float speedInchesPerSecond = SmartDashboard::GetNumber("speedInchesPerSecond", Autonomous::SpeedInchesPerSecond);
	float fullRotationTime = SmartDashboard::GetNumber("fullRotationTime", Autonomous::FullRotationTime);

	// Will be removed in end build
	section3.setPeriod(1, (struct Period) {static_cast<float>(90 / speedInchesPerSecond),1,1});
	section3.setPeriod(2, (struct Period) {static_cast<float>(0),0});
	section3.setPeriod(3, (struct Period) {static_cast<float>(0),0});
	section3.setPeriod(4, (struct Period) {static_cast<float>(0),0});
	section3.setPeriod(5, (struct Period) {static_cast<float>(0),0});

	section3.applySchedule(autoTimer.Get(), driveBase);

	autoAimVisual();
}
void Robot::scenario3()
{
	printf("senario3 \n");
	float speedInchesPerSecond = SmartDashboard::GetNumber("speedInchesPerSecond", Autonomous::SpeedInchesPerSecond);
	float fullRotationTime = SmartDashboard::GetNumber("fullRotationTime", Autonomous::FullRotationTime);

		printf("\n");
	}
}

void Robot::scenario4()
{
	printf("scenario4 \n");
	float speedInchesPerSecond = SmartDashboard::GetNumber("speedInchesPerSecond", Autonomous::SpeedInchesPerSecond);
	float fullRotationTime = SmartDashboard::GetNumber("fullRotationTime", Autonomous::FullRotationTime);

	// Will be removed in end build
	section3.setPeriod(1, (struct Period) {static_cast<float>(90 / speedInchesPerSecond),1,1});
	section3.setPeriod(2, (struct Period) {static_cast<float>(fullRotationTime / 4.0),-1,1});
	section3.setPeriod(3, (struct Period) {static_cast<float>(277.4 / speedInchesPerSecond * 0.15625),1,1});
	section3.setPeriod(4, (struct Period) {static_cast<float>(0),0});
	section3.setPeriod(5, (struct Period) {static_cast<float>(0),0});

	section3.applySchedule(autoTimer.Get(), driveBase);

	autoAimVisual();

}*/

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
}

START_ROBOT_CLASS(Robot)
