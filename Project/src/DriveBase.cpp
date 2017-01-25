#include "DriveBase.h"

DriveBase::DriveBase(float _maxSpeed, float _maxBoostSpeed, int leftPort, int rightPort) :
                    maxSpeed(_maxSpeed), maxBoostSpeed(_maxBoostSpeed),
			        leftMotor(leftPort), rightMotor(rightPort)
{
	rightMotor.SetInverted(true);
}

void DriveBase::drive(float leftSpeed, float rightSpeed, bool boost)
{
	leftSpeed = leftSpeed * fabs(leftSpeed) * (boost ? maxBoostSpeed : maxSpeed);
	rightSpeed = rightSpeed * fabs(rightSpeed) * (boost ? maxBoostSpeed : maxSpeed);

	leftMotor.Set(leftSpeed);
	rightMotor.Set(rightSpeed);
}

void DriveBase::driveForward(float speed)
{
	leftMotor.Set(speed);
	rightMotor.Set(speed);
}

void DriveBase::stop()
{
	leftMotor.Set(0);
	rightMotor.Set(0);
}
