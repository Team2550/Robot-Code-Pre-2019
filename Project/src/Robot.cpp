#include "Timer.h"
#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
				 driveBase(),
				 shooter(),
				 lift()
{
	decreaseShooterSpeedDown = false;
	increaseShooterSpeedDown = false;

	climbToggleHold = false;
	climbToggle = false;

	inchesPerSecond = Autonomous::inchesPerSecond;
	oneEigtheeTime = Autonomous::oneEigtheeTime;

	normalDrive = DriveType::Normal;
	backwardsDrive = DriveType::Backwards;
}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	//timeSinceStart.Start();
	SmartDashboard::PutNumber("inchesPerSecond", inchesPerSecond);
	SmartDashboard::PutNumber("oneEigtheeTime", oneEigtheeTime);

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
	autoTimer.Start();

	inchesPerSecond = SmartDashboard::GetNumber("inchesPerSecond", inchesPerSecond);
	oneEigtheeTime = SmartDashboard::GetNumber("oneEigtheeTime", oneEigtheeTime);

	airshipFrontVerticalTime = 93.3 / inchesPerSecond;
	airshipBackVerticalTime = 185.3 / inchesPerSecond;
	feildHorizTime = 277.4 / inchesPerSecond;
	horizStretchA = feildHorizTime * .15625;

	autoDriveTimes[0] = airshipFrontVerticalTime * .8;                //verticalStretchA
	autoDriveTimes[1] = oneEigtheeTime / 2;                           //ninteeTime
	autoDriveTimes[2] = horizStretchA;                                //horizStretchA
	autoDriveTimes[3] = oneEigtheeTime / 2;                           //ninteeTime
	autoDriveTimes[4] = airshipFrontVerticalTime - autoDriveTimes[0]; //verticalStretchB

	driveBase.setReversed(false);
}

void Robot::AutonomousPeriodic()
{
	/* ========== blind autonomous ========== */

	if(indx > MAX_NUM_AUTO_DRIVE_TIME)
	{
		return;
		driveBase.stop();
	}
	else if(indx == 1 || indx == 3)
	{
		autoTurn = true;
	}
	else
	{
		autoTurn = false;
	}

	if(autoTurn == false)
	{
		if(autoTimer.Get() < autoDriveTimes[indx])
		{
			driveBase.drive(1,1);
		}
		else
		{
			autoTimer.Reset();
			driveBase.stop();
			indx++;
		}
	}
	else
	{
		if(autoTimer.Get() <= autoDriveTimes[indx])
		{
			driveBase.drive(1,-1); //Turn right
		}
		else
		{
			autoTimer.Reset();
			driveBase.stop();
			indx++;
		}
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

	if (driveController.GetRawButton(Controls::TankDrive::flip)){
		autoTimer.Start();
		if(autoTimer.Get() >= oneEigtheeTime){
				driveBase.drive(1,-1);
				driveBase.stop();
			    }
		autoTimer.Stop();
	}

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
