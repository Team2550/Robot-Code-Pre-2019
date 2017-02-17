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
	{
		shooter.shoot();
		shooter.blend();
	}
	else
	{
		shooter.stop();
		shooter.stopBlend();
	}

	/* ========== Lift ========== */
	if(perifController.GetRawButton(Controls::Peripherals::Climb))
		lift.raise();
	else if(perifController.GetRawAxis(Controls::Peripherals::ClimbDown) > 0.5)
		lift.lower();
	else
		lift.stop();

	/* ==================== */
	printf("\n");
}

START_ROBOT_CLASS(Robot)
