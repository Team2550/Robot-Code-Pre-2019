#include "Drive.h"

Drive::Drive(float _maxSpeed, int leftPort, int rightPort) :
            maxSpeed(_maxSpeed), leftMotor(leftPort), rightMotor(rightPort)
{
	rightMotor.SetInverted(true);
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

void Drive::teleopPeriodic(float leftSpeed, float rightSpeed, int distanceFromTarget)
{
	deadzone(leftSpeed);
	deadzone(rightSpeed);

	leftMotor.Set(leftSpeed * maxSpeed);
	rightMotor.Set(rightSpeed * maxSpeed);
}
