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
	SmartDashboard::PutNumber("Left Forwards Ratio", Speeds::DriveBase::LeftPowerRatioForwards);
	SmartDashboard::PutNumber("Right Forwards Ratio", Speeds::DriveBase::RightPowerRatioForwards);
	SmartDashboard::PutNumber("Left Backwards Ratio", Speeds::DriveBase::LeftPowerRatioBackwards);
	SmartDashboard::PutNumber("Right Backwards Ratio", Speeds::DriveBase::RightPowerRatioBackwards);
}

void Robot::AutonomousInit()
{
	driveBase.setReversed(false);

	autoTimer.Reset();
	autoTimer.Start();
}

void Robot::AutonomousPeriodic()
{

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
}

START_ROBOT_CLASS(Robot)
