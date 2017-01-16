#include "Drive.h"

Drive::Drive(float _maxSpeed, float _maxBoostSpeed, int leftPort, int rightPort) :
            maxSpeed(_maxSpeed), maxBoostSpeed(_maxBoostSpeed),
			leftMotor(leftPort), rightMotor(rightPort)
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

void Drive::teleopPeriodic(float leftSpeed, float rightSpeed, bool boost)
{
	deadzone(leftSpeed);
	deadzone(rightSpeed);

	leftSpeed = leftSpeed * fabs(leftSpeed) * (boost ? maxBoostSpeed : maxSpeed);
	rightSpeed = rightSpeed * fabs(rightSpeed) * (boost ? maxBoostSpeed : maxSpeed);

	leftMotor.Set(leftSpeed);
	rightMotor.Set(rightSpeed);
}
