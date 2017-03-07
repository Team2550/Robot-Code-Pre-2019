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

	normalDrive = DriveType::Normal;
	backwardsDrive = DriveType::Backwards;
}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	//timeSinceStart.Start();
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

	driveBase.setReversed(true);
}

void Robot::AutonomousPeriodic()
{
	/* ========== blind autonomous ========== */

	int airshipFrontVerticalTime = Autonomous::inchesPerSecond *  93.3;
	int airshipBackVerticalTime = Autonomous::inchesPerSecond * 185.3;
	int verticalStretchA = airshipFrontVerticalTime * .8;
	int verticalStretchB = airshipFrontVerticalTime - verticalStretchA;
	int feildHorizTime = Autonomous::inchesPerSecond * 277.4;
	int horizStretchA = feildHorizTime * .15625;
	int ninteeTime = Autonomous::oneEigtheeTime / 2;
	autoTimer.Start();
	if(autoTimer.Get() >= verticalStretchA){
		driveBase.drive(1,1);
		driveBase.stop();
	    }
	autoTimer.Reset();
	if(autoTimer.Get() >= ninteeTime){
			driveBase.drive(1,-1);
			driveBase.stop();
		    }
	autoTimer.Reset();
	if(autoTimer.Get() >= horizStretchA){
			driveBase.drive(1,1);
			driveBase.stop();
		    }
	autoTimer.Reset();
	if(autoTimer.Get() >= verticalStretchB){
			driveBase.drive(1,1);
			driveBase.stop();
			}
	autoTimer.Stop();

	if (udpReceiver.getUDPDataAge() < 1.0)
	{
		printf("New UDP data:");

		float data[UDP::DataCount];
		udpReceiver.getUDPData(data);

		for (int i = 0; i < UDP::DataCount; i++)
		{
			printf(i > 0 ? ", " : " ");
			printf(std::to_string(data[i]).c_str());
		}

		printf(", Age: ");
		printf(std::to_string(udpReceiver.getUDPDataAge()).c_str());

		printf("\n");
	}

	printf("\n");

	/* ========== DriveBase ========== */
	if(autoTimer.Get() <= 3) // Placeholder time
	{
		driveBase.drive(0.8);
	}
	else
	{
		autoAim();
	}

	/** RECOMMENDED PSUEDO CODE
	 *
	 * (Segment timed periods using if structure used above)
	 *
	 * Move forward for x seconds
	 * Then...
	 * Run autoAim until end of autonomous
	 */
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
	if (udpReceiver.getUDPDataAge() < 1.0)
	{
		printf("New UDP data:");

		float data[UDP::DataCount];
		udpReceiver.getUDPData(data);

		for (int i = 0; i < UDP::DataCount; i++)
		{
			printf(i > 0 ? ", " : " ");
			printf(std::to_string(data[i]).c_str());
		}

		printf(", Age: ");
		printf(std::to_string(udpReceiver.getUDPDataAge()).c_str());

		printf("\n");
	}

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

	/*if (driveController.GetRawButton(Controls::TankDrive::flip)){
		autoTimer.Start();
		if(autoTimer.Get() >= Autonomous::oneEigtheeTime){
				driveBase.drive(1,-1);
				driveBase.stop();
			    }
		autoTimer.Stop();
	}*/

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

	/* ========== Auto Aim ========== */
	/** RECOMMENDED PSUEDO CODE
	 *
	 * (Segment timed periods using if structure used above)
	 *
	 * If control is pressed, run autoAim
	 */
	/*if (driveController.GetRawButton(Controls::TankDrive::AutoAim))
	{
		autoAim();
	}*/
}

void Robot::autoAim()
{
	// Get data
	float data[UDP::DataCount];
	udpReceiver.getUDPData(data);

	bool isDataGood = udpReceiver.getUDPDataAge() < 2.0 && udpReceiver.getUDPDataIsReal();

	if (!isDataGood) // If data isn't good, rotate blindly
		driveBase.drive(-0.5, 0.5);
	else if (data[UDP::Index::HorizAngle] > 5) // Target is to the right, rotate clockwise
		driveBase.drive(0.3, -0.3);
	else if (data[UDP::Index::HorizAngle] < -5) // Target is to the left, rotate counter-clockwise
		driveBase.drive(-0.3, 0.3);
	else // Target is near center
	{
		if (data[UDP::Index::Distance] > 5) // Target is far, approach
			driveBase.drive(0.3);
		else // Arrived
			driveBase.stop();
	}
}

START_ROBOT_CLASS(Robot)
