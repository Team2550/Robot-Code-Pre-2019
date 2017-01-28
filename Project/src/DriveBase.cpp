#include "DriveBase.h"

DriveBase::DriveBase(Joystick& _driveController, Joystick& _perifController,
                     float _maxSpeed, float _maxBoostSpeed) :
                     driveController(_driveController), perifController(_perifController),
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

    deadzone(leftSpeed);
    deadzone(rightSpeed);

    leftSpeed = leftSpeed * fabs(leftSpeed) * (boost ? maxBoostSpeed : maxSpeed);
    rightSpeed = rightSpeed * fabs(rightSpeed) * (boost ? maxBoostSpeed : maxSpeed);

    leftMotor.Set(leftSpeed);
    rightMotor.Set(rightSpeed);
}
