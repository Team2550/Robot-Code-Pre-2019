#include "DriveBase.h"
#include "Utility.h"

DriveBase::DriveBase() : leftMotor(Ports::TankDrive::Left), rightMotor(Ports::TankDrive::Right)
{
    leftMotor.SetInverted(true);
    isReversed = false;
}

float DriveBase::getLeftSpeed()
{
	if (isReversed)
		return -rightMotor.Get() * (rightMotor.GetInverted() ? -1 : 1);
	else
		return leftMotor.Get() * (leftMotor.GetInverted() ? -1 : 1);
}

float DriveBase::getRightSpeed()
{
	if (isReversed)
		return -leftMotor.Get() * (leftMotor.GetInverted() ? -1 : 1);
	else
		return rightMotor.Get() * (rightMotor.GetInverted() ? -1 : 1);
}

void DriveBase::drive(float speed)
{
	drive(speed, speed);
}

void DriveBase::drive(float leftSpeed, float rightSpeed)
{
	if(isReversed)
	{
		float temp = leftSpeed;
		leftSpeed = -rightSpeed;
		rightSpeed = -temp;
	}

	leftMotor.Set(leftSpeed);
	rightMotor.Set(rightSpeed);
}

void DriveBase::stop()
{
	drive(0, 0);
}

void DriveBase::applyTrim(float leftForwardsRatio, float rightForwardsRatio,
    		              float leftBackwardsRatio, float rightBackwardsRatio)
{
	float leftSpeed = getLeftSpeed();
	float rightSpeed = getRightSpeed();

	leftSpeed *= (leftSpeed > 0) ? leftForwardsRatio : leftBackwardsRatio;
	rightSpeed *= (rightSpeed > 0) ? rightForwardsRatio : rightBackwardsRatio;

	drive(leftSpeed, rightSpeed);
}

void DriveBase::setReversed(bool reverse)
{
	isReversed = reverse;
}
