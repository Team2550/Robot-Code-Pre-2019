#include <AutoController.h>

AutoController::AutoController(DriveBase* driveBase, Gyro* gyroscope)
{
	this->driveBase = driveBase;
	this->gyroscope = gyroscope;

	instructionSet.steps = NULL;
	instructionSet.count = 0;

	currentInstruction = 0;
	instructionStartTime = 0;
	instructionStartDistance = 0;
	instructionTargetAngle = 0;
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

	driveBase->ResetDistance();
	gyroscope->Reset();

	currentInstruction = 0;
	instructionStartTime = 0;
	instructionStartDistance = 0;
	instructionTargetAngle = 0;
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
	double leftSpeed = (instructionSet.steps + currentInstruction)->leftSpeed;
	double rightSpeed = (instructionSet.steps + currentInstruction)->rightSpeed;

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
		std::cout << "Driving to " << target << " and angle of " << instructionTargetAngle << std::endl;
		instructionCompleted = AutoDriveToDist( leftSpeed, rightSpeed, target, instructionTargetAngle );
		break;

	case ROTATE_DEG:
		target += instructionTargetAngle;
	case ROTATE_TO:
		std::cout << "Rotating to " << target << std::endl;
		instructionCompleted = AutoRotateToAngle( leftSpeed, rightSpeed, target );
		break;

	case RESET_DIST_0:
		driveBase->ResetDistance();
		driveBase->Stop();
		std::cout << "Reset dist" << std::endl;
		instructionCompleted = true;
		break;

	//case RESET_DIST_ULTRA:
	//	driveBase->ResetDistance(); // TODO: Add a way to reset the distance to the current ultrasonic reading
	//	driveBase->Stop();
	//	instructionCompleted = true;
	//	break;

	default:
		driveBase->Stop();
	}

	if (instructionCompleted)
	{
		instructionStartTime = timer.Get();
		instructionStartDistance = driveBase->GetRightDistance(); //(driveBase->GetLeftDistance() + driveBase->GetRightDistance()) / 2;

		if (instructionType == ROTATE_DEG || instructionType == ROTATE_TO)
			instructionTargetAngle = target;

		currentInstruction++;
	}

	// Check if all instructions are complete
	if (currentInstruction >= instructionSet.count)
		return true;
	else
		return false;
}

bool AutoController::AutoDriveToDist( double leftSpeed, double rightSpeed, double targetDistance, double targetAngle )
{
	// Get sensor data
	double currentDistance = driveBase->GetRightDistance(); //(driveBase->GetLeftDistance() + driveBase->GetRightDistance()) / 2; // Average of left and right distances.
	double currentAngle = gyroscope->GetAngle();

	// Get the angle that the robot has drifted from its target as a percentage out of 90 degrees
	double angleOffsetPercent = (currentAngle - targetAngle) / 10;

	// Limit angle offset to range of -90 to 90 (-100% to 100%)
	if (angleOffsetPercent > 1)
		angleOffsetPercent = 1;
	if (angleOffsetPercent < -1)
		angleOffsetPercent = -1;

	// Set speed offset to the speed multiplied by the percent that the robot is to 90 degrees off target
	double leftSpeedOffset = 1 - angleOffsetPercent;
	double rightSpeedOffset = 1 + angleOffsetPercent;

	// Scale the speed based on distance to target
	double speedMultiplier;

	// Passed target, back up
	if (currentDistance > targetDistance)
		speedMultiplier = -1;

	// Slow down on approach
	if (fabs(targetDistance - currentDistance) < 12)
		speedMultiplier = fabs(targetDistance - currentDistance) / 12;

	driveBase->Drive(leftSpeed * speedMultiplier * leftSpeedOffset, rightSpeed * speedMultiplier * rightSpeedOffset);

	// Returns true if distance to target is less than 6 (1 foot range)
	return abs( currentDistance - targetDistance ) < 6;
}

bool AutoController::AutoRotateToAngle( double leftSpeed, double rightSpeed, double targetAngle )
{
	// Get sensor data
	double currentAngleOffset = gyroscope->GetAngle() - targetAngle;

	if ( currentAngleOffset > 3 )
		driveBase->Drive(-leftSpeed, rightSpeed);
	else if ( currentAngleOffset < 3 )
		driveBase->Drive(leftSpeed, -rightSpeed);
	else
		driveBase->Drive(0, 0);

	// Return true if angle is within range of (targetAngle - 5, targetAngle + 5))
	return abs( currentAngleOffset ) < 5;
}
