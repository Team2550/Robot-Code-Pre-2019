#include "DriveBase.h"
#include "Utility.h"

DriveBase::DriveBase(int leftPort, int rightPort, float _maxSpeed, float _maxBoostSpeed, float _maxTurtleSpeed) :
                     leftMotor(leftPort), rightMotor(rightPort),
                     maxSpeed(_maxSpeed), maxBoostSpeed(_maxBoostSpeed), maxTurtleSpeed(_maxTurtleSpeed)
{
    rightMotor.SetInverted(true);
}

void DriveBase::drive(float leftSpeed, float rightSpeed, bool boost, bool turtle)
{
    // Basic exponential speed control
    leftSpeed *= fabs(leftSpeed);
    rightSpeed *= fabs(rightSpeed);

    if(boost)
    {
    	leftSpeed *= maxBoostSpeed;
    	rightSpeed *= maxBoostSpeed;
    }
    else if(turtle)
    {
    	leftSpeed *= maxTurtleSpeed;
    	rightSpeed *= maxTurtleSpeed;
    }
    else
    {
    	leftSpeed *= maxSpeed;
    	rightSpeed *= maxSpeed;
    }

    // Accounts for physical offsets on the drive base
    leftSpeed = leftSpeed > 0 ? leftSpeed * 1.05 : leftSpeed;
    leftSpeed = leftSpeed < 0 ? leftSpeed * 0.99 : leftSpeed;

    leftMotor.Set(leftSpeed);
    rightMotor.Set(rightSpeed);
}

void DriveBase::driveForward()
{
	leftMotor.Set(maxSpeed);
	rightMotor.Set(maxSpeed);
}

void DriveBase::driveBackwards()
{
	leftMotor.Set(-maxSpeed);
	rightMotor.Set(-maxSpeed);
}

void DriveBase::turnLeft()
{
	leftMotor.Set(-maxTurtleSpeed);
	rightMotor.Set(maxTurtleSpeed);
}

void DriveBase::turnRight()
{
	leftMotor.Set(maxTurtleSpeed);
	rightMotor.Set(-maxTurtleSpeed);
}

void DriveBase::stop()
{
	leftMotor.Set(0);
	rightMotor.Set(0);
}
