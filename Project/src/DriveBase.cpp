#include "DriveBase.h"

DriveBase::DriveBase(int leftMotorPort, int rightMotorPort,
					 int leftEncoderPortA, int leftEncoderPortB,
					 int rightEncoderPortA, int rightEncoderPortB,
					 double encoderDistancePerPulse) :
	leftMotor(leftMotorPort), rightMotor(rightMotorPort),
	leftEncoder(leftEncoderPortA, leftEncoderPortB, Encoder::EncodingType::k2X),
	rightEncoder(rightEncoderPortA, rightEncoderPortB, Encoder::EncodingType::k2X)
{
	leftMotor.SetInverted(false);
	rightMotor.SetInverted(true);

	// Set encoder reversal for one side
	leftEncoder.SetReverseDirection(false);
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

	leftTargetDistance = 0;
	rightTargetDistance = 0;
	threshold = DEFAULT_AUTODRIVE_THRESHOLD_INCHES;

	nextCallback = nullptr;
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

void DriveBase::UpdateAutoDrive()
{
	// TODO: Set motor speeds based on what is needed to acheive goals.
	Drive(100, 100);

	RunCallback(false);
}

void DriveBase::AutoDriveTo(double left,
		double right, double thresholdInches, Utility::ActionCallback callback )
{
	leftTargetDistance = left;
	rightTargetDistance = right;
	threshold = thresholdInches;

	SetNextCallback(callback);
}

void DriveBase::AutoDriveDist(double distance,
		double thresholdInches, Utility::ActionCallback callback )
{
	AutoDriveTo(GetLeftDistance() + distance, GetRightDistance() + distance, thresholdInches, callback);
}

void DriveBase::AutoRotate(double degrees,
		double thresholdInches, Utility::ActionCallback callback )
{
	double radians = (degrees / 180) * 3.14159265;
	double radius = 30 / 2; // Drive base width / 2
	double arcLengthInches = radians * radius;

	AutoDriveTo(GetLeftDistance() + arcLengthInches, GetRightDistance() - arcLengthInches, thresholdInches, callback);
}

void DriveBase::RunCallback(bool actionCompleted)
{
	if (nextCallback != nullptr)
		nextCallback(actionCompleted);

	nextCallback = nullptr;
}

void DriveBase::SetNextCallback(Utility::ActionCallback func)
{
	RunCallback(false);

	nextCallback = func;
}
