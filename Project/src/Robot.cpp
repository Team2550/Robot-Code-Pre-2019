#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
				 driveBase(),
				 shooter(),
				 lift()
{

}

Robot::~Robot()
{

}

void Robot::RobotInit()
{

}

void Robot::AutonomousInit()
{

	driveBase.setReversed(true);
}

void Robot::AutonomousPeriodic()
{
	/* ========== udpReceiver ========== */
	udpReceiver.checkUDP();

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
	/*if(autoTimer.Get() <= 3) // Placeholder time
	{
		driveBase.drive(0.8);
	}
	else
	{
		autoAim();
	}*/

	driveBase.drive(1.0);

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
	bool boost = driveController.GetRawButton(Controls::TankDrive::Boost);
	bool turtle = driveController.GetRawButton(Controls::TankDrive::Turtle);
	float speed = turtle ? Speeds::DriveBase::Turtle : (boost ? Speeds::DriveBase::Boost : Speeds::DriveBase::Normal);
	driveBase.drive(leftSpeed *speed,
					rightSpeed * speed);

	/* ========== Shooter ========== */
	if(perifController.GetRawButton(Controls::Peripherals::Shoot))
		shooter.shoot();
	else
		shooter.stop();

	/* ========== Lift ========== */
	if(perifController.GetRawButton(Controls::Peripherals::Climb))
		lift.lift();
	else if(perifController.GetRawAxis(Controls::Peripherals::ClimbDown) > 0.5)
		lift.lower();
	else
		lift.stop();

	/* ==================== */
}

START_ROBOT_CLASS(Robot)
