#include "DriveBase.h"
#include "Utility.h"

DriveBase::DriveBase() : leftMotor(Ports::TankDrive::Left), rightMotor(Ports::TankDrive::Right)
{
    leftMotor.SetInverted(true);
    isReversed = false;
}

void DriveBase::drive(float speed)
{
	leftMotor.Set(speed);
	rightMotor.Set(speed);
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
	leftMotor.Set(0);
	rightMotor.Set(0);
}

void DriveBase::setReversed(bool reverse)
{
	isReversed = reverse;
}
