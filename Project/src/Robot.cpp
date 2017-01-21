#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driver(0), driveBase(0.4, 0.8, 1, 0), shooter(2)
{
	shooterSpeed = 0.5;
	xIsPressed = false;
	yIsPressed = false;
}

Robot::~Robot()
{

}

void Robot::RobotInit()
{

}


void Robot::AutonomousInit()
{
	driveBase.autoInit();
}

void Robot::AutonomousPeriodic()
{
	driveBase.autoPeriodic();
}

void Robot::TeleopInit()
{
	driveBase.teleopInit();
}

void Robot::TeleopPeriodic()
{
	driveBase.teleopPeriodic(-driver.GetRawAxis(xbox::axis::leftY),
			                 -driver.GetRawAxis(xbox::axis::rightY),
					    	 driver.GetRawButton(xbox::btn::rb));

	SmartDashboard::GetNumber("shooterSpeed", shooterSpeed);
	if(driver.GetRawButton(xbox::btn::x) && !xIsPressed)
	{
		shooterSpeed -= 0.01;
		xIsPressed = true;
	}
	if(driver.GetRawButton(xbox::btn::y) && !yIsPressed)
	{
		shooterSpeed += 0.01;
		yIsPressed = true;
	}
	SmartDashboard::PutNumber("shooterSpeed", shooterSpeed);

	if(driver.GetRawButton(xbox::btn::b))
		shooter.stop();
	else if(driver.GetRawButton(xbox::btn::a))
		shooter.shoot(shooterSpeed);
}

void Robot::TestPeriodic()
{

}

START_ROBOT_CLASS(Robot)
