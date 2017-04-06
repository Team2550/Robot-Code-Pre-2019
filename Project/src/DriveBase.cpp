#include "DriveBase.h"
#include "Utility.h"

/*!
 * \brief Constructor for DriveBase
 *
 * Initializes leftMotor and rightMotor to the appropriate ports, sets rightMotor as
 * inverted (because it's inverted on the Robot), and specifies that our initial driving style
 * is not reversed
 */
DriveBase::DriveBase() : leftMotor(Ports::TankDrive::Left), rightMotor(Ports::TankDrive::Right)
{
    rightMotor.SetInverted(true);
    isReversed = false;
}

/*!
 * \brief Gives the speed that the left motor is set to
 *
 * \return The speed that the left motor is set to
 */
float DriveBase::getLeftSpeed()
{
	if (isReversed)
		return -rightMotor.Get() * (rightMotor.GetInverted() ? -1 : 1);
	else
		return leftMotor.Get() * (leftMotor.GetInverted() ? -1 : 1);
}

/*!
 * \brief Gives the speed that the right motor is set to
 *
 * \return The speed that the right motor is set to
 */
float DriveBase::getRightSpeed()
{
	if (isReversed)
		return -leftMotor.Get() * (leftMotor.GetInverted() ? -1 : 1);
	else
		return rightMotor.Get() * (rightMotor.GetInverted() ? -1 : 1);
}

/*!
 * \brief Drives in a straight line at the given speed
 *
 * \param[in] speed The speed to drive at from -1 to 1
 */
void DriveBase::drive(float speed)
{
	drive(speed, speed);
}

/*!
 * \brief Drives at the given speeds
 *
 * Sets the left and right motors to the given speeds, allowing full
 * control of the Robot's drive train
 * \param[in] leftSpeed The speed to drive the left side at from -1 to 1
 * \param[in] rightSpeed The speed to drive the right side at from -1 to 1
 */
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

/*!
 * \brief Stops the Robot from moving
 */
void DriveBase::stop()
{
	drive(0, 0);
}

/*!
 * \brief Applies trim to the drive train
 *
 * This applies trim to the speeds that the Robot is currently driving at;
 * the trim is lost when another function is called that sets the speed
 * of the drive train. The trim values are just multiplied by the speeds that the
 * Robot is driving at.
 * \param[in] leftForwardsRatio The trim to apply to the left side when driving forwards
 * \param[in] rightForwardsRatio The trim to apply to the right side when driving forwards
 * \param[in] leftBackwardsRatio The trim to apply to the left side when driving backwards
 * \param[in] rightBackwardsRatio The trim to apply to the right side when driving backwards
 */
void DriveBase::applyTrim(float leftForwardsRatio, float rightForwardsRatio,
    		              float leftBackwardsRatio, float rightBackwardsRatio)
{
	float leftSpeed = leftMotor.Get() * (leftMotor.GetInverted() ? -1 : 1);
	float rightSpeed = rightMotor.Get() * (rightMotor.GetInverted() ? -1 : 1);

	leftSpeed *= (leftSpeed > 0) ? leftForwardsRatio : leftBackwardsRatio;
	rightSpeed *= (rightSpeed > 0) ? rightForwardsRatio : rightBackwardsRatio;

	leftMotor.Set(leftSpeed);
	rightMotor.Set(rightSpeed);
}

/*!
 * \brief Changes which end of the Robot is the front
 *
 * \param[in] reverse Whether or not the back of the Robot should be considered the front
 */
void DriveBase::setReversed(bool reverse)
{
	isReversed = reverse;
}

/*!
 * \brief Tells which end of the Robot is the front
 *
 * \return True if and only if the back of the Robot is being treated as the front
 */
bool DriveBase::getReversed()
{
	return isReversed;
}

/*!
 * \brief Gives the average current draw of the drive motors
 *
 * To be clear, by average I mean the median of the current draws of each individual
 * motor
 * \param[in] pdp A PDP object that will be used to find the current draw of the motors
 * \return The average current draw of the drive motors
 */
float DriveBase::getAmps(PowerDistributionPanel& pdp)
{
	return (pdp.GetCurrent(Ports::PDP::LeftMotor1) + pdp.GetCurrent(Ports::PDP::LeftMotor2) +
		    pdp.GetCurrent(Ports::PDP::RightMotor1) + pdp.GetCurrent(Ports::PDP::RightMotor2)) / 4;
}
