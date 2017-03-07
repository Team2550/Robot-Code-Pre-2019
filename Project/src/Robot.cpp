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
}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	//timeSinceStart.Start();
}

void Robot::AutonomousInit()
{

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



	printf("\n");
}

void Robot::TeleopInit()
{
	/* ========== DriveBase ========== */
	driveBase.stop();
}

void Robot::TeleopPeriodic()
{
	/* ========== udpReceiver ========== */
//	udpReceiver.checkUDP();
//
//	if (udpReceiver.getUDPDataAge() < 1.0)
//	{
//		printf("New UDP data:");
//
//		for (int i = 0; i < UDP::DataCount; i++)
//		{
//			printf(i > 0 ? ", " : " ");
//			printf(std::to_string(udpReceiver.getUDPData()[i]).c_str());
//		}
//
//		printf(", Age: ");
//		printf(std::to_string(udpReceiver.getUDPDataAge()).c_str());
//
//		printf("\n");
//	}

	/* ========== DriveBase ========== */
	float leftSpeed = Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Left));
	float rightSpeed = Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Right));
	bool boost = driveController.GetRawButton(Controls::TankDrive::Boost);
	bool turtle = driveController.GetRawButton(Controls::TankDrive::Turtle);
	float speed = turtle ? Speeds::TankDrive::Turtle : (boost ? Speeds::TankDrive::Boost : Speeds::TankDrive::Normal);
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

	if (driveController.GetRawButton(Controls::TankDrive::flip)){
		autoTimer.Start();
		if(autoTimer.Get() >= Autonomous::oneEigtheeTime){
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

	/* ==================== */
}

START_ROBOT_CLASS(Robot)
