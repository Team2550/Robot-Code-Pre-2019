#include "DriveBase.h"
#include "Utility.h"

DriveBase::DriveBase(int leftMotorPort, int rightMotorPort,
					 int leftEncoderPortA, int leftEncoderPortB,
					 int rightEncoderPortA, int rightEncoderPortB,
					 double encoderDistancePerPulse) :
	leftMotor(leftMotorPort), rightMotor(rightMotorPort),
	leftEncoder(leftEncoderPortA, leftEncoderPortB, Encoder::EncodingType::k2X),
	rightEncoder(rightEncoderPortA, rightEncoderPortB, Encoder::EncodingType::k2X)
{
	rightMotor.SetInverted(true);
	rightEncoder.SetReverseDirection(true);

	// Distance per pulse from encoder.
	leftEncoder.SetDistancePerPulse(encoderDistancePerPulse);
	rightEncoder.SetDistancePerPulse(encoderDistancePerPulse);

	// Length of time needed to determine whether robot is stopped (seconds).
	leftEncoder.SetMaxPeriod(.1);
	rightEncoder.SetMaxPeriod(.1);

	// Minimum speed to determine if robot is stopped (distance units/second).
	leftEncoder.SetMinRate(1);
	rightEncoder.SetMinRate(1);

	leftTrim = 1;
	rightTrim = 1;
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

bool DriveBase::IsStopped()
{
	return leftEncoder.GetStopped() && rightEncoder.GetStopped();
}

void DriveBase::ResetDistance()
{
	leftEncoder.Reset();
	rightEncoder.Reset();
}

double DriveBase::GetLeftDistance()
{
	return leftEncoder.GetDistance();
}

double DriveBase::GetRightDistance()
{
	return rightEncoder.GetDistance();
}

void DriveBase::SetTrim(float leftTrim, float rightTrim)
{
	this->leftTrim = leftTrim;
	this->rightTrim = rightTrim;
}
