#include "DriveBase.h"
#include "Utility.h"

DriveBase::DriveBase() : leftMotor(Ports::TankDrive::Left), rightMotor(Ports::TankDrive::Right)
{
    leftMotor.SetInverted(true);
    isReversed = false;
}

void DriveBase::drive(float speed)
{
	leftMotor.Set(speed * Speeds::DriveBase::LeftPowerRatio);
	rightMotor.Set(speed * Speeds::DriveBase::RightPowerRatio);
}

void DriveBase::drive(float leftSpeed, float rightSpeed)
{
	if(isReversed)
	{
		float temp = leftSpeed;
		leftSpeed = -rightSpeed;
		rightSpeed = -temp;
	}

	leftMotor.Set(leftSpeed * Speeds::DriveBase::LeftPowerRatio);
	rightMotor.Set(rightSpeed * Speeds::DriveBase::RightPowerRatio);
}

void DriveBase::stop()
{
	leftMotor.Set(0);
	rightMotor.Set(0);
}

void DriveBase::setReversed(bool reverse)
{
	isReversed = reverse;
}
