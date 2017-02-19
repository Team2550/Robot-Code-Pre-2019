#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1),
                 udpReceiver(),
				 udpSender("2550-programming0.local", 8890), // "raspberrypi.local"
				 driveBase(),
				 shooter(),
				 lift()
{
	sentEndMessage = false;
}

Robot::~Robot()
{

}

void Robot::RobotInit()
{

}

void Robot::AutonomousInit()
{
	matchTimer.Start();
	matchTimer.Reset();
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
}

void Robot::TeleopInit()
{
	matchTimer.Start();
	matchTimer.Reset();

	/* ========== DriveBase ========== */
	driveBase.stop();
}

void Robot::TeleopPeriodic()
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

	/* ========== udpReceiver ========== */
	if (matchTimer.Get() > Other::MatchLength && !sentEndMessage)
	{
		udpSender.sendUDPData("end");
		sentEndMessage = true;
	}
}

START_ROBOT_CLASS(Robot)
