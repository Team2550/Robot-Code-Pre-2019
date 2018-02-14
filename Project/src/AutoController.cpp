#include <AutoController.h>

AutoController::AutoController(DriveBase* driveBase, ADXRS450_Gyro* gyroscope)
{
	this->driveBase = driveBase;
	this->gyroscope = gyroscope;

	instructionSet.steps = NULL;
	instructionSet.count = 0;

	currentInstruction = 0;
	instructionStartTime = 0;
	instructionStartDistance = 0;
	instructionStartAngle = 0;
}

AutoController::~AutoController()
{

}

void AutoController::Init(InstructionSet instructionSet)
{
	timer.Reset();
	timer.Start();

	this->instructionSet.steps = instructionSet.steps;
	this->instructionSet.count = instructionSet.count;

	currentInstruction = 0;
	instructionStartTime = 0;
	instructionStartDistance = (driveBase->GetLeftDistance() + driveBase->GetRightDistance()) / 2;
	instructionStartAngle = gyroscope->GetAngle();
}

bool AutoController::Execute()
{
	if (instructionSet.steps == NULL || currentInstruction >= instructionSet.count)
		return true;

	// Gets set to true if instruction is complete
	bool instructionCompleted = false;

	// Instruction Information
	InstructionType instructionType = (instructionSet.steps + currentInstruction)->type;
	double target = (instructionSet.steps + currentInstruction)->target;
	double speed = (instructionSet.steps + currentInstruction)->speed;

	switch (instructionType)
	{
	case WAIT_TIME:
		target += instructionStartTime;
	case WAIT_UNTIL:
		driveBase->Stop();
		instructionCompleted = (timer.Get() >= target);
		break;

	case DRIVE_DIST:
		target += instructionStartDistance;
	case DRIVE_TO:
		instructionCompleted = AutoDriveToDist( speed, target, instructionStartAngle );
		break;

	case ROTATE_DEG:
		target += instructionStartAngle;
	case ROTATE_TO:
		instructionCompleted = AutoRotateToAngle( speed, target );
		break;

	case RESET_DIST_0:
		driveBase->ResetDistance();
		driveBase->Stop();
		instructionCompleted = true;
		break;

	case RESET_DIST_ULTRA:
		driveBase->ResetDistance(); // TODO: Add a way to reset the distance to the current ultrasonic reading
		driveBase->Stop();
		instructionCompleted = true;
		break;

	default:
		driveBase->Stop();
	}

	if (instructionCompleted)
	{
		instructionStartTime = timer.Get();
		instructionStartDistance = (driveBase->GetLeftDistance() + driveBase->GetRightDistance()) / 2;
		instructionStartAngle = gyroscope->GetAngle();

		currentInstruction++;
	}

	// Check if all instructions are complete
	if (currentInstruction >= instructionSet.count)
		return true;
	else
		return false;
}

bool AutoController::AutoDriveToDist( double speed, double targetDistance, double targetAngle )
{
	// Get sensor data
	double currentDistance = (driveBase->GetLeftDistance() + driveBase->GetRightDistance()) / 2; // Average of left and right distances.
	double currentAngle = gyroscope->GetAngle();

	// Get the angle that the robot has drifted from its target as a percentage out of 90 degrees
	double angleOffsetPercent = (currentAngle - targetAngle) / 90;

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
	if (currentDistance > targetDistance)
		scaledSpeed *= -1;

	if (abs(currentDistance - targetDistance) < 24)
		scaledSpeed *= abs(currentDistance - targetDistance) / 24;

	driveBase->Drive(scaledSpeed - rightSpeedOffset, scaledSpeed + rightSpeedOffset);

	// Returns true if distance to target is less than 6 (1 foot range)
	return abs( currentDistance - targetDistance ) < 6;
}

bool AutoController::AutoRotateToAngle( double speed, double targetAngle )
{
	// Get sensor data
	double currentAngle = gyroscope->GetAngle();

	if ( currentAngle > targetAngle + 5 )
		driveBase->Drive(speed, -speed);
	else if ( currentAngle < targetAngle - 5 )
		driveBase->Drive(-speed, speed);

	// Return true if angle is within range of (targetAngle - 5, targetAngle + 5))
	return abs( currentAngle - targetAngle ) < 5;
}
