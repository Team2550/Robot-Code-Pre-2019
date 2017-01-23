#include "Robot.h"

// driver: (int) xBox controller number
// driveBase:  (float) max power, (float) max boost power, (int) left motor port,
//             (int) right motor port
Robot::Robot() : driveController(0), perifController(1), driveBase(0.4, 0.8, 1, 0)
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
	driveBase.autoInit();
}

void Robot::AutonomousPeriodic()
{
	driveBase.autoPeriodic();
}

void Robot::TeleopInit()
{
	driveBase.teleopInit();

	udpReceive();
}

void Robot::TeleopPeriodic()
{
	driveBase.teleopPeriodic(-driveController.GetRawAxis(xbox::axis::leftY),
			                 -driveController.GetRawAxis(xbox::axis::rightY),
							 driveController.GetRawButton(xbox::btn::rb));

	printf("As numbers:");

	for (int i = 0; i < 4; i++)
		printf(std::to_string(dataFromPi[i]).c_str());

	printf("\n");
}

START_ROBOT_CLASS(Robot)
