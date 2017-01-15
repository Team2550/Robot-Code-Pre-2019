#include "Drive.h"
#include "Utility.cpp"

Drive::Drive(int frontLeftPort, int frontRightPort, int backLeftPort, int backRightPort) :
				frontLeftMotor(frontLeftPort), frontRightMotor(frontRightPort),
				backLeftMotor(backLeftPort), backRightMotor(backLeftPort)
{

}

void Drive::autoInit()
{

}

void Drive::autoPeriodic()
{

}

void Drive::teleopInit()
{

}

void Drive::teleopPeriodic(float speed, float rotation)
{
	deadzone(speed);
	deadzone(rotation);

	frontLeftMotor.Set(speed + rotation);
	frontRightMotor.Set(speed - rotation);
	backLeftMotor.Set(speed + rotation);
	backRightMotor.Set(speed - rotation);
}
