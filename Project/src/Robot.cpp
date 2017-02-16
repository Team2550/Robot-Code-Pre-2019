#include "Timer.h"
#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
				 driveBase(),
				 shooter(),
				 lift(),
				 btnHoldTimer()

{
	btnHoldTimer.Start();
	btnHold = false;
}

Robot::~Robot()
{
	btnHoldTimer.Stop();
}

void Robot::RobotInit()
{

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
}

void Robot::TeleopPeriodic()
{
	/* ========== udpReceiver ========== */
	udpReceiver.checkUDP();

	printf("As numbers:");

	for (int i = 0; i < 4; i++)
	{
		printf(i > 0 ? ", " : " ");
		printf(std::to_string(udpReceiver.getUDPData()[i]).c_str());
	}

	printf("\n");

	/* ========== DriveBase ========== */
	float leftSpeed = Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Left));
	float rightSpeed = Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Right));
	bool boost = driveController.GetRawButton(Controls::TankDrive::Boost);
	bool turtle = driveController.GetRawButton(Controls::TankDrive::Turtle);
	float speed = turtle ? Speeds::TankDrive::Turtle : (boost ? Speeds::TankDrive::Boost : Speeds::TankDrive::Normal);
	driveBase.drive(leftSpeed *speed,
					rightSpeed * speed);

	/* ========== Shooter ========== */
	if(perifController.GetRawButton(Controls::Peripherals::Shoot))
		shooter.shoot();
	else
		shooter.stop();

	/* ========== Lift/In-Feed ========== */
	if(perifController.GetRawButton(Controls::Peripherals::ClimbOnOff))
	{
		//Don't proceed until the btn has been released
		//Prevents code from setting the btnHold twice if the user releases the btn slowly
		while(perifController.GetRawButton(Controls::Peripherals::ClimbOnOff))
		{
			//If the user still holds the btn  after one sec, break to let the program continue
			if(btnHoldTimer.HasPeriodPassed(1) == true)
			{
				break;
			}
		}

		if(btnHold == false)
		{
			btnHold = true;
		}
		else
		{
			btnHold = false;
		}
	}

	if(btnHold == true)
		lift.lift();
	else if(perifController.GetRawAxis(Controls::Peripherals::ClimbTrig) > 0.5)
		lift.lift();
	else
		lift.stop();


	/* ==================== */
	printf("\n");
}

START_ROBOT_CLASS(Robot)
