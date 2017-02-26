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
	/* ========== Blind Autonomous ========== */
	//float speedInchesPerSecond = SmartDashboard::GetNumber("speedInchesPerSecond", Autonomous::SpeedInchesPerSecond);
	//float fullRotationTime = SmartDashboard::GetNumber("fullRotationTime", Autonomous::FullRotationTime);

	choreographer.applySchedule(autoTimer.Get(), driveBase);
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
