#include "DriveBase.h"

DriveBase::DriveBase(Joystick& _driveController, Joystick& _perifController,
		  	  	  	 UDP_Receiver& _udpReceiver,
                     float _maxSpeed, float _maxBoostSpeed) :
                     driveController(_driveController), perifController(_perifController),
					 udpReceiver(_udpReceiver),
                     maxSpeed(_maxSpeed), maxBoostSpeed(_maxBoostSpeed),
                     leftMotor(Ports::TankDrive::Left), rightMotor(Ports::TankDrive::Right)
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

void DriveBase::TeleopPeriodic()
{
    float leftSpeed = driveController.GetRawAxis(Controls::TankDrive::Left);
    float rightSpeed = driveController.GetRawAxis(Controls::TankDrive::Right);
    bool boost = driveController.GetRawButton(Controls::TankDrive::Boost);
    bool autoAiming = driveController.GetRawButton(Controls::TankDrive::AutoAim);
    int* openCVData = udpReceiver.getUDPData();

    Utility::deadzone(leftSpeed);
    Utility::deadzone(rightSpeed);

    leftSpeed = leftSpeed * fabs(leftSpeed) * (boost ? maxBoostSpeed : maxSpeed);
    rightSpeed = rightSpeed * fabs(rightSpeed) * (boost ? maxBoostSpeed : maxSpeed);

    leftMotor.Set(leftSpeed);
    rightMotor.Set(rightSpeed);

	if (autoAiming == true)
	{
		printf("aiming");
		if(openCVData[1] > 10){
			driveForward(0.5);
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
