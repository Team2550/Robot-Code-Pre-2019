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
	climbToggleHold = false;
	climbToggle = false;
}

Robot::~Robot()
{

}

void Robot::RobotInit()
{

}

void Robot::AutonomousInit()
{
	autoTimer.Reset();
	autoTimer.Start();
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
	if(autoTimer.Get() <= 3.000)
	{
		driveBase.drive(-0.4);
	}
	else if(autoTimer.Get() >= 8.000 && autoTimer.Get() <= 9.000)
	{
		driveBase.drive(0.4);
	}
	else if(autoTimer.Get() > 9.000 && autoTimer.Get() <= 11.000)
	{
		driveBase.drive(0.15, 1);
	}
	else
	{
		driveBase.stop();
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
	float speed = turtle ? Speeds::TankDrive::Turtle : (boost ? Speeds::TankDrive::Boost : Speeds::TankDrive::Normal);
	driveBase.drive(leftSpeed *speed,
					rightSpeed * speed);

	/* ========== Shooter ========== */
	if(perifController.GetRawButton(Controls::Peripherals::Shoot))
		shooter.shoot();
	else
		shooter.stop();

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
		lift.lift();
	else
		lift.stop();

	/* ========== Auto Aim ========== */
	/** RECOMMENDED PSUEDO CODE
	 *
	 * (Segment timed periods using if structure used above)
	 *
	 * If control is pressed, run autoAim
	 */
}

void Robot::autoAim()
{
	/** RECOMMENDED PSUEDO CODE
	 *
	 * (Segment timed periods using if structure used above)
	 *
	 * Get UDP data and store it in an array
	 * Rotate clockwise or counterclockwise (based on field configuration) until target is found
	 * 		and the offset value is near zero
	 * Go backwards until distance is small
	 *
	 * // Needs to compensate for tilted target, but this information must be added to UDP
	 */
}

START_ROBOT_CLASS(Robot)
