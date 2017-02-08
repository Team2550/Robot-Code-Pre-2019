#include "DriveBase.h"
#include "Utility.h"

DriveBase::DriveBase() : leftMotor(Ports::TankDrive::Left), rightMotor(Ports::TankDrive::Right)
{
    rightMotor.SetInverted(true);
}

void DriveBase::drive(float speed)
{
	leftMotor.Set(speed);
	rightMotor.Set(speed);
}

void DriveBase::drive(float leftSpeed, float rightSpeed)
{
	leftMotor.Set(leftSpeed);
	rightMotor.Set(rightSpeed);
}

void DriveBase::stop()
{
	leftMotor.Set(0);
	rightMotor.Set(0);
}
