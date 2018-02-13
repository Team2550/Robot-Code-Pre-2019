#include "AutoStrategy.h"

AutoStrategy::AutoStrategy(Instruction* instructions, unsigned int instructionCount)
{
	this->instructions = instructions;
	this->instructionCount = instructionCount;

	currentInstruction = 0;
	instructionStartTime = 0;
	instructionStartDistance = 0;
	instructionStartAngle = 0;
}

AutoStrategy::~AutoStrategy()
{
	delete[] instructions;
}

bool AutoStrategy::Execute( DriveBase& driveBase, ADXRS450_Gyro& gyroscope, Timer& autoTimer )
{
	if (currentInstruction >= instructionCount)
		return true;

	// Sensor data
	double time = autoTimer.Get();
	double angle = gyroscope.GetAngle();
	double distance = (driveBase.GetLeftDistance() + driveBase.GetRightDistance()) / 2; // Average of left and right distances.

	bool instructionCompleted = false; // Gets set to true if instruction is complete

	// Instruction Information
	InstructionType instructionType = (instructions + currentInstruction)->type;
	double target = (instructions + currentInstruction)->target;
	double speed = (instructions + currentInstruction)->speed;

	switch (instructionType)
	{
	case WAIT_TIME:
		target += instructionStartTime;
	case WAIT_UNTIL:
		driveBase.Stop();
		instructionCompleted = (time >= target);
		break;

	case DRIVE_DIST:
		target += instructionStartDistance;
	case DRIVE_TO:
		instructionCompleted = AutoDriveToDist( speed, distance, target, angle, instructionStartAngle, driveBase );
		break;

	case ROTATE_DEG:
		target += instructionStartAngle;
	case ROTATE_TO:
		instructionCompleted = AutoRotateToAngle( speed, target, angle, driveBase );
		break;

	case RESET_DIST_0:
		driveBase.ResetDistance();
		distance = 0;
		driveBase.Stop();
		instructionCompleted = true;
		break;

	case RESET_DIST_ULTRA:
		driveBase.ResetDistance(); // TODO: Add a way to reset the distance to the current ultrasonic reading
		distance = 0;
		driveBase.Stop();
		instructionCompleted = true;
		break;

	case RESET_ANGLE:
		gyroscope.Reset();
		angle = 0;
		driveBase.Stop();
		instructionCompleted = true;
		break;

	default:
		driveBase.Stop();
	}

	if (instructionCompleted)
	{
		// Increment current instruction number and get next instruction's type
		InstructionType nextInstructionType = (instructions + (++currentInstruction))->type;

		instructionStartTime = time;
		instructionStartDistance = distance;
		instructionStartAngle = angle;
	}

	return currentInstruction >= instructionCount; // Return true if strategy is complete
}

bool AutoStrategy::AutoDriveToDist( double speed, double distance, double angle, double targetDistance, double targetAngle, DriveBase& driveBase )
{
	// Get the angle that the robot has drifted from its target as a percentage out of 90 degrees
	double angleOffsetPercent = (angle - targetAngle) / 90;

	// Limit angle offset to range of -90 to 90 (-100% to 100%)
	if (angleOffsetPercent > 1)
		angleOffsetPercent = 1;
	if (angleOffsetPercent < -1)
		angleOffsetPercent = -1;

	// Set speed offset to the speed multiplied by the percent that the robot is to 90 degrees off target
	double rightSpeedOffset = angleOffsetPercent * speed;

	// Scale the speed based on distance to target
	double scaledSpeed = speed;

	// Passed target, back up
	if (distance > targetDistance)
		scaledSpeed *= -1;

	if (abs(distance - targetDistance) < 24)
		scaledSpeed *= abs(distance - targetDistance) / 24;

	driveBase.Drive(scaledSpeed - rightSpeedOffset, scaledSpeed + rightSpeedOffset);

	// Returns true if distance to target is less than 6 (1 foot range)
	return abs( distance - targetDistance ) < 6;
}

bool AutoStrategy::AutoRotateToAngle( double speed, double angle, double targetAngle, DriveBase& driveBase )
{
	if ( angle > targetAngle + 5 )
		driveBase.Drive(speed, -speed);
	else if ( angle < targetAngle - 5 )
		driveBase.Drive(-speed, speed);

	// Return true if angle is within range of (targetAngle - 5, targetAngle + 5))
	return abs( angle - targetAngle ) < 5;
}
