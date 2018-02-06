#include "DriveBase.h"
#include "Utility.h"

DriveBase::DriveBase(int leftMotorPort, int rightMotorPort) : leftMotor(leftMotorPort), rightMotor(rightMotorPort)
{
	leftMotor.SetInverted(false);
	rightMotor.SetInverted(true);

	leftTrim = 1;
	rightTrim = 1;
}

double DriveBase::GetLeftSpeed()
{
	if (leftMotor.GetInverted())
		return -leftMotor.Get();
	else
		return leftMotor.Get();
}

double DriveBase::GetRightSpeed()
{
	if (rightMotor.GetInverted())
		return -rightMotor.Get();
	else
		return rightMotor.Get();
}

void DriveBase::Drive(double leftSpeed, double rightSpeed)
{
	leftMotor.Set(leftSpeed * leftTrim);
	rightMotor.Set(rightSpeed * rightTrim);
}

void DriveBase::Drive(double speed)
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
