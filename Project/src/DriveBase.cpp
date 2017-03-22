#include "DriveBase.h"
#include "Utility.h"

DriveBase::DriveBase() : leftMotor(Ports::TankDrive::Left), rightMotor(Ports::TankDrive::Right)
{
    rightMotor.SetInverted(true);
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

void DriveBase::stop()
{
	drive(0, 0);
}

void DriveBase::applyTrim(float leftForwardsRatio, float rightForwardsRatio,
    		              float leftBackwardsRatio, float rightBackwardsRatio)
{
	float leftSpeed = leftMotor.Get() * (leftMotor.GetInverted() ? -1 : 1);
	float rightSpeed = rightMotor.Get() * (rightMotor.GetInverted() ? -1 : 1);

	leftSpeed *= (leftSpeed > 0) ? leftForwardsRatio : leftBackwardsRatio;
	rightSpeed *= (rightSpeed > 0) ? rightForwardsRatio : rightBackwardsRatio;

	leftMotor.Set(leftSpeed);
	rightMotor.Set(rightSpeed);

	printf("Set motors: left = ");
	printf(std::to_string(leftMotor.Get()).c_str());
	printf(", right = ");
	printf(std::to_string(rightMotor.Get()).c_str());
	printf("\n");
}

void DriveBase::setReversed(bool reverse)
{
	isReversed = reverse;
}

bool DriveBase::getReversed()
{
	return isReversed;
}

float DriveBase::getAmps(PowerDistributionPanel& pdp)
{
	return (pdp.GetCurrent(Ports::PDP::LeftMotor1) + pdp.GetCurrent(Ports::PDP::LeftMotor2) +
		    pdp.GetCurrent(Ports::PDP::RightMotor1) + pdp.GetCurrent(Ports::PDP::RightMotor2)) / 4;
}
