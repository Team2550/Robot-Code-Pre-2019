#include "Timer.h"
#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
                 choreographer(Autonomous::PeriodCount, Autonomous::Timetable),
				 driveBase(),
				 shooter(),
				 lift()
{
	normalDrive = DriveType::Normal;
	backwardsDrive = DriveType::Backwards;

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
	SmartDashboard::PutNumber("speedInchesPerSecond", Autonomous::SpeedInchesPerSecond);
	SmartDashboard::PutNumber("fullRotationTime", Autonomous::FullRotationTime);

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
	autoTimer.Reset();
	autoTimer.Start();

	driveBase.setReversed(false);
}

void Robot::AutonomousPeriodic()
{
	float senario = SmartDashboard::GetNumber("choose your scenario:", Autonomous::scenario);
	SmartDashboard::PutNumber("you chose scenario:", Autonomous::scenario);
	if(senario == 1)
	{
		scenario1();
	}
	if(senario == 2)
	{
		scenario2();
	}
	if(senario == 3)
	{
		scenario3();
	}
	if(senario == 4)
	{
		scenario4();
	}
}
void Robot::scenario1()
{
	printf("senario1 \n");
}
void Robot::scenario2()
{
	printf("scenario2 \n");
	/* ========== Blind Autonomous ========== */
	float speedInchesPerSecond = SmartDashboard::GetNumber("speedInchesPerSecond", Autonomous::SpeedInchesPerSecond);
	float fullRotationTime = SmartDashboard::GetNumber("fullRotationTime", Autonomous::FullRotationTime);

	// Will be removed in end build
	section3.setPeriod(1, (struct Period) {static_cast<float>(90 / speedInchesPerSecond),1,1});
	section3.setPeriod(2, (struct Period) {static_cast<float>(0),0});
	section3.setPeriod(3, (struct Period) {static_cast<float>(0),0});
	section3.setPeriod(4, (struct Period) {static_cast<float>(0),0});
	section3.setPeriod(5, (struct Period) {static_cast<float>(0),0});

	section3.applySchedule(autoTimer.Get(), driveBase);

	autoAim();
}
void Robot::scenario3()
{
	printf("senario3 \n");
	/* ========== Blind Autonomous ========== */
	float speedInchesPerSecond = SmartDashboard::GetNumber("speedInchesPerSecond", Autonomous::SpeedInchesPerSecond);
	float fullRotationTime = SmartDashboard::GetNumber("fullRotationTime", Autonomous::FullRotationTime);

	// Will be removed in end build
	section3.setPeriod(1, (struct Period) {static_cast<float>(93.3 / speedInchesPerSecond * 0.8),1,1});
	section3.setPeriod(2, (struct Period) {static_cast<float>(fullRotationTime / 2.0),-1,1});
	section3.setPeriod(3, (struct Period) {static_cast<float>(277.4 / speedInchesPerSecond * 0.15625),1,1});
	section3.setPeriod(4, (struct Period) {static_cast<float>(fullRotationTime / 2.0),-1,1});
	section3.setPeriod(5, (struct Period) {static_cast<float>(93.3 / speedInchesPerSecond * 0.2),-1,1});

	section3.applySchedule(autoTimer.Get(), driveBase);

	autoAim();

}

void Robot::scenario4()
{
	printf("scenario4 \n");
	/* ========== Blind Autonomous ========== */
	float speedInchesPerSecond = SmartDashboard::GetNumber("speedInchesPerSecond", Autonomous::SpeedInchesPerSecond);
	float fullRotationTime = SmartDashboard::GetNumber("fullRotationTime", Autonomous::FullRotationTime);

	// Will be removed in end build
	section3.setPeriod(1, (struct Period) {static_cast<float>(90 / speedInchesPerSecond),1,1});
	section3.setPeriod(2, (struct Period) {static_cast<float>(fullRotationTime / 4.0),-1,1});
	section3.setPeriod(3, (struct Period) {static_cast<float>(277.4 / speedInchesPerSecond * 0.15625),1,1});
	section3.setPeriod(4, (struct Period) {static_cast<float>(0),0});
	section3.setPeriod(5, (struct Period) {static_cast<float>(0),0});

	section3.applySchedule(autoTimer.Get(), driveBase);

	autoAim();

}
void Robot::autoAim()
{
	printf("aiming \n");
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
