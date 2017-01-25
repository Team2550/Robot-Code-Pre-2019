#include "Robot.h"
#include "Utility.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1), driveBase(0.4, 0.8, 1, 0), shooter(0.5, 2)
{

}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	shooter.RobotInit();
}

void Robot::AutonomousInit()
{
	shooter.AutoInit();
}

void Robot::AutonomousPeriodic()
{
	shooter.AutoPeriodic();
}

void Robot::TeleopInit()
{
	shooter.TeleopInit();
}

void Robot::TeleopPeriodic()
{
	drive(deadzone(-driveController.GetRawAxis(Controls::TankDrive::Left)),
	      deadzone(-driveController.GetRawAxis(Controls::TankDrive::Right)),
	      deadzone( driveController.GetRawButton(Controls::TankDrive::Boost)));

	shooter.TeleopPeriodic(perifController.GetRawButton(Controls::Peripherals::Shoot),
	                       perifController.GetRawButton(Controls::Peripherals::StopShoot),
						   perifController.GetRawButton(Controls::Peripherals::IncreaseShootSpeed),
						   perifController.GetRawButton(Controls::Peripherals::DecreaseShootSpeed));

}

START_ROBOT_CLASS(Robot)

void Robot::drive(float leftSpeed, float rightSpeed, bool boost)
{
	driveBase.drive(leftSpeed, rightSpeed, boost);
}
