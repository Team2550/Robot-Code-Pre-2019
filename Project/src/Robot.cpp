#include "Robot.h"

Robot::Robot() : driver(0), drive(1, 3, 2, 0), chooser()
{

}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
	chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
	SmartDashboard::PutData("Auto Modes", chooser);
}

/**
 * This autonomous (along with the chooser code above) shows how to select between different autonomous modes
 * using the dashboard. The sendable chooser code works with the Java SmartDashboard. If you prefer the LabVIEW
 * Dashboard, remove all of the chooser code and uncomment the GetString line to get the auto name from the text box
 * below the Gyro
 *
 * You can add additional auto modes by adding additional comparisons to the if-else structure below with additional strings.
 * If using the SendableChooser make sure to add them to the chooser code above as well.
 */
void Robot::AutonomousInit()
{
	/*autoSelected endif
= *((std::string*)chooser->GetSelected());
	//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
	std::cout << "Auto selected: " << autoSelected << std::endl;

	if(autoSelected == autoNameCustom){
		//Custom Auto goes here
	} else {
		//Default Auto goes here
	}*/

	drive.autoInit();
}

void Robot::AutonomousPeriodic()
{
	if(autoSelected == autoNameCustom){
		//Custom Auto goes here
	} else {
		//Default Auto goes here
	}

	drive.autoPeriodic();
}

void Robot::TeleopInit()
{
	drive.teleopInit();
}

void Robot::TeleopPeriodic()
{
	drive.teleopPeriodic(driver.GetRawAxis(xbox::axis::leftY),
			             driver.GetRawAxis(xbox::axis::leftX));
}

void Robot::TestPeriodic()
{
	lw->Run();
}

START_ROBOT_CLASS(Robot)
