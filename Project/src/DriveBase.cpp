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
		return rightMotor.Get();
	else
		return -leftMotor.Get();
}

float DriveBase::getRightSpeed()
{
	if (isReversed)
		return -leftMotor.Get();
	else
		return rightMotor.Get();
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

	leftMotor.Set(leftSpeed);// * (leftSpeed > 0 ? Speeds::DriveBase::LeftPowerRatioForwards : Speeds::DriveBase::LeftPowerRatioBackwards));
	rightMotor.Set(rightSpeed);// * (rightSpeed > 0 ? Speeds::DriveBase::RightPowerRatioForwards : Speeds::DriveBase::RightPowerRatioBackwards));
}

void DriveBase::stop()
{
	drive(0, 0);
}

void DriveBase::setReversed(bool reverse)
{
	isReversed = reverse;
}
