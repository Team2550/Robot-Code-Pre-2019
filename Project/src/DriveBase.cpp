#include "DriveBase.h"

DriveBase::DriveBase(float _maxSpeed, float _maxBoostSpeed, int leftPort, int rightPort) :
                    maxSpeed(_maxSpeed), maxBoostSpeed(_maxBoostSpeed),
			        leftMotor(leftPort), rightMotor(rightPort)
{
	rightMotor.SetInverted(true);
}

void DriveBase::autoInit()
{

}

void DriveBase::autoPeriodic()
{

}

void DriveBase::teleopInit()
{
	leftMotor.Set(0);
	rightMotor.Set(0);
}

void DriveBase::teleopPeriodic(float leftSpeed, float rightSpeed, bool boost)
{
	deadzone(leftSpeed);
	deadzone(rightSpeed);

	leftSpeed = leftSpeed * fabs(leftSpeed) * (boost ? maxBoostSpeed : maxSpeed);
	rightSpeed = rightSpeed * fabs(rightSpeed) * (boost ? maxBoostSpeed : maxSpeed);

	leftMotor.Set(leftSpeed);
	rightMotor.Set(rightSpeed);
}
