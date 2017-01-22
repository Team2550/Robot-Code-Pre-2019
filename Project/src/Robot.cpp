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
	driveBase.RobotInit();
}


void Robot::AutonomousInit()
{
	driveBase.AutoInit();
}

void Robot::AutonomousPeriodic()
{
	driveBase.AutoPeriodic();
}

void Robot::TeleopInit()
{
	driveBase.TeleopInit();
}

void Robot::TeleopPeriodic()
{
	driveBase.TeleopPeriodic(-driver.GetRawAxis(xbox::axis::leftY),
			                 -driver.GetRawAxis(xbox::axis::rightY),
					    	 driver.GetRawButton(xbox::btn::rb));

	if(driver.GetRawButton(xbox::btn::x) && !xIsPressed)
	{
		shooterSpeed -= 0.01;
		xIsPressed = true;
	}
	else if(!driver.GetRawButton(xbox::btn::x))
		xIsPressed = false;
	if(driver.GetRawButton(xbox::btn::y) && !yIsPressed)
	{
		shooterSpeed += 0.01;
		yIsPressed = true;
	}
	else if(!driver.GetRawButton(xbox::btn::y))
			yIsPressed = false;
	SmartDashboard::PutNumber("shooterSpeed", shooterSpeed);

	if(driver.GetRawButton(xbox::btn::b))
		shooter.stop();
	else if(driver.GetRawButton(xbox::btn::a))
		shooter.shoot(shooterSpeed);
}

START_ROBOT_CLASS(Robot)
