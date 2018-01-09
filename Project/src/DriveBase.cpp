#include "DriveBase.h"
#include "Utility.h"

DriveBase::DriveBase(int leftMotorPort, int rightMotorPort) : leftMotor(leftMotorPort), rightMotor(rightMotorPort)
{
	rightMotor.SetInverted(true);
	isReversed = false;
}

float DriveBase::GetLeftSpeed()
{
	if (isReversed)
		return -rightMotor.Get() * (rightMotor.GetInverted() ? -1 : 1);
	else
		return leftMotor.Get() * (leftMotor.GetInverted() ? -1 : 1);
}

float DriveBase::GetRightSpeed()
{
	if (isReversed)
		return -leftMotor.Get() * (leftMotor.GetInverted() ? -1 : 1);
	else
		return rightMotor.Get() * (rightMotor.GetInverted() ? -1 : 1);
}

void DriveBase::Drive(float leftSpeed, float rightSpeed)
{
	if (isReversed)
	{
		leftMotor.Set(-rightSpeed);
		rightMotor.Set(-leftSpeed);
	}
	else
	{
		leftMotor.Set(leftSpeed);
		rightMotor.Set(rightSpeed);
	}
}

void DriveBase::Drive(float speed)
{
	Drive(speed, speed);
}

void DriveBase::Stop()
{
	Drive(0, 0);
}

void DriveBase::ApplyTrim(float leftForwardsRatio, float rightForwardsRatio,
                          float leftBackwardsRatio, float rightBackwardsRatio)
{
	float leftSpeed = leftMotor.Get() * (leftMotor.GetInverted() ? -1 : 1);
	float rightSpeed = rightMotor.Get() * (rightMotor.GetInverted() ? -1 : 1);

	leftSpeed *= (leftSpeed > 0) ? leftForwardsRatio : leftBackwardsRatio;
	rightSpeed *= (rightSpeed > 0) ? rightForwardsRatio : rightBackwardsRatio;

	leftMotor.Set(leftSpeed);
	rightMotor.Set(rightSpeed);
}

void DriveBase::SetReversed(bool reverse)
{
	isReversed = reverse;
}

bool DriveBase::GetReversed()
{
	return isReversed;
}
