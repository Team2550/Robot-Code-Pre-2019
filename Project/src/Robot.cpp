#include "Robot.h"

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
	driveBase.RobotInit();
	shooter.RobotInit();
}


void Robot::AutonomousInit()
{
	driveBase.AutoInit();
	shooter.AutoInit();
}

void Robot::AutonomousPeriodic()
{
	driveBase.AutoPeriodic();
	shooter.AutoPeriodic();
}

void Robot::TeleopInit()
{
	driveBase.TeleopInit();
	shooter.TeleopInit();
}

void Robot::TeleopPeriodic()
{
	driveBase.TeleopPeriodic(-driveController.GetRawAxis(xbox::axis::leftY),
	                         -driveController.GetRawAxis(xbox::axis::rightY),
	                         driveController.GetRawButton(xbox::btn::rb));
	shooter.TeleopPeriodic(perifController.GetRawButton(xbox::btn::b),
	                       perifController.GetRawButton(xbox::btn::a),
						   perifController.GetRawButton(xbox::btn::y),
						   perifController.GetRawButton(xbox::btn::x));
}

START_ROBOT_CLASS(Robot)
