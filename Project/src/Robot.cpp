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
	/* ========== udpReceiver ========== */
	udpReceiver.checkUDP();

	printf("As numbers: ");

	for (int i = 0; i < 4; i++)
	{
		printf(std::to_string(udpReceiver.getUDPData()[i]).c_str());
		printf(", ");
	}

	printf("\n");
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
	/* ========== udpReceiver ========== */
	udpReceiver.checkUDP();

	if (udpReceiver.getUDPDataAge() < 1.0)
	{
		printf("New UDP data:");

		for (int i = 0; i < UDP::DataCount; i++)
		{
			printf(i > 0 ? ", " : " ");
			printf(std::to_string(udpReceiver.getUDPData()[i]).c_str());
		}

		printf(", Age: ");
		printf(std::to_string(udpReceiver.getUDPDataAge()).c_str());

		printf("\n");
	}

	/* ========== DriveBase ========== */
	float leftSpeed = Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Left));
	float rightSpeed = Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Right));
	bool boost = driveController.GetRawButton(Controls::TankDrive::Boost);
	bool turtle = driveController.GetRawButton(Controls::TankDrive::Turtle);
	float speed = turtle ? Speeds::TankDrive::Turtle : (boost ? Speeds::TankDrive::Boost : Speeds::TankDrive::Normal);
	driveBase.drive(leftSpeed *speed,
					rightSpeed * speed);

	/* ========== Shooter ========== */
	if (perifController.GetRawButton(Controls::Peripherals::Shoot))
	{
		shooter.shoot();

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
			printf(std::to_string(Speeds::Shooter::ShooterSpeed +
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
			printf(std::to_string(Speeds::Shooter::ShooterSpeed +
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

	/* ==================== */
}

START_ROBOT_CLASS(Robot)
