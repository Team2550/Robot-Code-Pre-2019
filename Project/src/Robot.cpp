#include "Robot.h"

Robot::Robot() : driveController(0), perifController(1),
				 autoController(&driveBase, &gyroscope),
                 ultrasonic(0, (5 / 4.88) * (1000 / 25.4), 1), // (5 mm / 4.88 mV) * (1/25.4 in/mm) * (1000 mV/V)
				 gyroscope(frc::SPI::Port::kOnboardCS0),
				 driveBase(0, 1, 0, 1, 2, 3, 6.07 * M_PI, 512) // Pulses per rotation is set by encoder DIP switch. 512 PPR uses DIP switch configuration 0001.

{
	axisTankLeft = xbox::axis::leftY;
	axisTankRight = xbox::axis::rightY;
	buttonBoost = xbox::btn::rb;
	buttonTurtle = xbox::btn::lb;

	boostPressTime = -999;

	selectedAutoStrategy = NULL;

	UpdatePreferences();

}

Robot::~Robot()
{

}

void Robot::RobotInit()
{
	std::cout << "Calibrating gyro..." << std::endl;
	gyroscope.Calibrate();
	std::cout << "Gyro calibrated" << std::endl;

	// Start Video Stream
	cs::UsbCamera camera = CameraServer::GetInstance()->StartAutomaticCapture();
	camera.SetResolution(320, 240);
}
