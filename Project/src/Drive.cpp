#include "Drive.h"

Drive::Drive(int leftPort, int rightPort) :
            leftMotor(leftPort), rightMotor(rightPort)
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
	leftMotor.Set(0);
	rightMotor.Set(0);
}

void Drive::teleopPeriodic(float speed, float rotation)
{
	deadzone(speed);
	deadzone(rotation);

	leftMotor.Set(speed + rotation);
	rightMotor.Set(speed - rotation);
}
