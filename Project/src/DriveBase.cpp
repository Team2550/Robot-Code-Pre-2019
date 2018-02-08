#include "DriveBase.h"
#include "Utility.h"

DriveBase::DriveBase(int leftMotorPort, int rightMotorPort) : leftMotor(leftMotorPort), rightMotor(rightMotorPort)
{
	rightMotor.SetInverted(true);

	leftTrim = 1;
	rightTrim = 1;

	lastActionCallback = nullptr;
}

float DriveBase::GetLeftSpeed()
{
	return leftMotor.Get() * (leftMotor.GetInverted() ? -1 : 1);
}

float DriveBase::GetRightSpeed()
{
	return rightMotor.Get() * (rightMotor.GetInverted() ? -1 : 1);
}

void DriveBase::Drive(float leftSpeed, float rightSpeed)
{
	leftMotor.Set(leftSpeed * leftTrim);
	rightMotor.Set(rightSpeed * rightTrim);
}

void DriveBase::Drive(float speed)
{
	Drive(speed, speed);
}

void DriveBase::Stop()
{
	Drive(0, 0);
}

void DriveBase::SetTrim(float leftTrim, float rightTrim)
{
	this->leftTrim = leftTrim;
	this->rightTrim = rightTrim;
}

void DriveBase::Rotate(float degrees, DriveActionCallback callback)
{
	if (lastActionCallback != nullptr)
		lastActionCallback();

	lastActionCallback = callback;
}
