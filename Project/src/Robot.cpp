#include "Robot.h"
#include "Utility.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1), driveBase(0.4, 0.8, 1, 0), shooter(0.8, 2)
{
	autoAiming = false;
}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	udpReceiver.RobotInit();
}

void Robot::AutonomousInit()
{
	udpReceiver.AutoInit();
}

void Robot::AutonomousPeriodic()
{
	udpReceiver.AutoPeriodic();
}

void Robot::TeleopInit()
{
	udpReceiver.TeleopInit();
}

void Robot::TeleopPeriodic()
{
	udpReceiver.TeleopPeriodic();

	if(autoAiming)
		autoDrive();
	else
	{
		drive(deadzone(-driveController.GetRawAxis(Controls::TankDrive::Left)),
		      deadzone(-driveController.GetRawAxis(Controls::TankDrive::Right)),
		      deadzone( driveController.GetRawButton(Controls::TankDrive::Boost)));
	}

	shoot(perifController.GetRawButton(Controls::Peripherals::Shoot),
	      perifController.GetRawButton(Controls::Peripherals::StopShoot),
	      perifController.GetRawButton(Controls::Peripherals::IncreaseShootSpeed),
	      perifController.GetRawButton(Controls::Peripherals::DecreaseShootSpeed));
}

START_ROBOT_CLASS(Robot)

void Robot::drive(float leftSpeed, float rightSpeed, bool boost)
{
	driveBase.drive(leftSpeed, rightSpeed, boost);
}

void Robot::shoot(bool shoot, bool stop, bool increaseSpeed, bool decreaseSpeed)
{
	static float shooterSpeed = 0.8;
	static bool increasedSpeed = false;
	static bool decreasedSpeed = false;

	if(decreaseSpeed && !decreasedSpeed)
	{
		shooterSpeed -= 0.01;
		decreasedSpeed = true;
	}
	else if(!decreaseSpeed)
		decreasedSpeed = false;

	if(increaseSpeed && !increasedSpeed)
	{
		shooterSpeed += 0.01;
		increasedSpeed = true;
	}
	else if(!increaseSpeed)
		increasedSpeed = false;

	frc::SmartDashboard::PutNumber("shooterSpeed", shooterSpeed);

	if(stop)
		shooter.stop();
	else if(shoot)
		shooter.shoot(shooterSpeed);
}

void Robot::autoDrive()
{
	int openCVData[] = udpReceiver.getUDPData();

	std::cout << "aiming" << std::endl;

	if(openCVData[1] > 10)
		driveBase.driveForward(.5);
	else
		driveBase.stop();
}
