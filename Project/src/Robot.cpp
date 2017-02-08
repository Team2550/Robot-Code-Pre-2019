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
	timer.Reset();
	timer.Start();
}

void Robot::AutonomousPeriodic()
{
	/* ========== udpReceiver ========== */
	udpReceiver.checkUDP();

	printf("As numbers:");

	for (int i = 0; i < 4; i++)
		printf(std::to_string(udpReceiver.getUDPData()[i]).c_str());

	printf("\n");

	/* ========== DriveBase ========== */
	if(timer.Get() <= 3.000)
	{
		driveBase.drive(-0.4);
	}
	else if(timer.Get() >= 8.000 && timer.Get() <= 9.000)
	{
		driveBase.drive(0.4);
	}
	else if(timer.Get() > 9.000 && timer.Get() <= 11.000)
	{
		driveBase.drive(0.15, 1);
	}
	else
	{
		driveBase.stop();
	}
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
		printf(std::to_string(udpReceiver.getUDPData()[i]).c_str());

	printf("\n");

	/* ========== DriveBase ========== */
	float leftSpeed = Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Left));
	float rightSpeed = Utility::deadzone(-driveController.GetRawAxis(Controls::TankDrive::Right));
	bool boost = driveController.GetRawButton(Controls::TankDrive::Boost);
	bool turtle = driveController.GetRawButton(Controls::TankDrive::Turtle);
	driveBase.drive(leftSpeed * (turtle ? 0.25 : (boost ? 0.8 : 0.4)),
					rightSpeed * (turtle ? 0.25 : (boost ? 0.8 : 0.4)));

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
}

START_ROBOT_CLASS(Robot)
