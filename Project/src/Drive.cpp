#include "Drive.h"

Drive::Drive(int frontLeftPort, int frontRightPort, int backLeftPort, int backRightPort) :
            frontLeftMotor(frontLeftPort), frontRightMotor(frontRightPort),
            backLeftMotor(backLeftPort), backRightMotor(backRightPort)
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
	frontLeftMotor.Set(0);
	frontRightMotor.Set(0);
	backLeftMotor.Set(0);
	backRightMotor.Set(0);
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
