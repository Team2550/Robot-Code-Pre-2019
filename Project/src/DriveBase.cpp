#include "DriveBase.h"

DriveBase::DriveBase(float _maxSpeed, float _maxBoostSpeed, int leftPort, int rightPort) :
                    maxSpeed(_maxSpeed), maxBoostSpeed(_maxBoostSpeed),
			        leftMotor(leftPort), rightMotor(rightPort)
{
	rightMotor.SetInverted(true);
}

void DriveBase::RobotInit()
{

}

void DriveBase::AutoInit()
{

}

void DriveBase::AutoPeriodic()
{

}

void DriveBase::TeleopInit()
{
	leftMotor.Set(0);
	rightMotor.Set(0);
}

void DriveBase::TeleopPeriodic(float leftSpeed, float rightSpeed, bool boost, bool autoaiming, int openCVData[])
{
	deadzone(leftSpeed);
	deadzone(rightSpeed);

	leftSpeed = leftSpeed * fabs(leftSpeed) * (boost ? maxBoostSpeed : maxSpeed);
	rightSpeed = rightSpeed * fabs(rightSpeed) * (boost ? maxBoostSpeed : maxSpeed);

	leftMotor.Set(leftSpeed);
	rightMotor.Set(rightSpeed);

	if(autoaiming=true){
		printf "aiming";
		if(openCVData[1] > 10){
			driveForward(.5);


		}
	}
}

void DriveBase::driveForward(float speed)
{
	leftMotor.Set(speed);
	rightMotor.Set(speed);
}

void DriveBase::stop()
{
	leftMotor.Set(0);
	rightMotor.Set(0);
}
