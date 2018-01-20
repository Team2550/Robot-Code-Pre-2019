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
	leftMotor.Set(leftSpeed);
	rightMotor.Set(rightSpeed);
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

void DriveBase::ResetLeftDistance()
{
	leftEncoder.Reset();
}

void DriveBase::ResetRightDistance()
{
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
