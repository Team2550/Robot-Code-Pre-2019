#include "AutoController.h"

AutoController::AutoController(DriveBase* driveBase, Bulldozer* bulldozer, Gyro* gyroscope)
{
	this->driveBase = driveBase;
	this->bulldozer = bulldozer;
	this->gyroscope = gyroscope;

	instructionSet.steps = NULL;
	instructionSet.count = 0;

	currentInstruction = 0;
	instructionStartTime = 0;
	instructionStartDistance = 0;
	instructionTargetAngle = 0;
	bulldozerExtended = false;
	bulldozerKicking = false;
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
	bulldozerExtended = false;
}

bool AutoController::Execute()
{
	if (instructionSet.steps == NULL || currentInstruction >= instructionSet.count)
		return true;

	// Gets set to true if instruction is complete
	bool instructionCompleted = false;

	// Instruction Information
	Instruction inst = *(instructionSet.steps + currentInstruction);
	std::cout << "Left Speed: " << inst.leftSpeed << " Right Speed: " << inst.rightSpeed << std::endl;

	switch (inst.type)
	{
	case WAIT_TIME:
		inst.target += instructionStartTime;
	case WAIT_UNTIL:
		driveBase->Drive(inst.leftSpeed, inst.rightSpeed);
		instructionCompleted = (timer.Get() >= inst.target);
		break;

	case DRIVE_DIST:
		inst.target += instructionStartDistance;
	case DRIVE_TO:
		std::cout << "Driving to " << inst.target << " and angle of " << instructionTargetAngle << std::endl;
		instructionCompleted = AutoDriveToDist( inst.leftSpeed, inst.rightSpeed, inst.target, instructionTargetAngle, inst.stopAtTarget );
		break;

	case ROTATE_DEG:
		inst.target += instructionTargetAngle;
	case ROTATE_TO:
		std::cout << "Rotating to " << inst.target << std::endl;
		instructionCompleted = AutoRotateToAngle( inst.leftSpeed, inst.rightSpeed, inst.target, inst.stopAtTarget );
		break;

	case RESET_DIST_0:
		driveBase->ResetDistance();
		std::cout << "Reset dist" << std::endl;
		instructionCompleted = true;
		break;

	case EXTEND:
		bulldozerExtended = true;
		instructionCompleted = true;
		break;

	case RETRACT:
		bulldozerExtended = false;
		instructionCompleted = true;
		break;

	case KICK:
		bulldozerKicking = true;
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

	if (bulldozerExtended)
		bulldozer->Extend();
	else
		bulldozer->Retract();

	if (bulldozerKicking)
		bulldozerKicking = !bulldozer->Kick(0);

	if (instructionCompleted)
	{
		instructionStartTime = timer.Get();
		instructionStartDistance = driveBase->GetRightDistance(); //(driveBase->GetLeftDistance() + driveBase->GetRightDistance()) / 2;

		if (inst.type == ROTATE_DEG || inst.type == ROTATE_TO)
			instructionTargetAngle = inst.target;

		currentInstruction++;
	}

	// Check if all instructions are complete
	if (currentInstruction >= instructionSet.count)
	{
		driveBase->Stop();
		return true;
	}
	else
		return false;
}

bool AutoController::AutoDriveToDist( double leftSpeed, double rightSpeed, double targetDistance, double targetAngle, bool stopAtTarget )
{
	// Get sensor data
	double currentDistance = driveBase->GetRightDistance(); //(driveBase->GetLeftDistance() + driveBase->GetRightDistance()) / 2; // Average of left and right distances.
	double currentAngle = gyroscope->GetAngle();

	// Get the angle that the robot has drifted from its target as a percentage out of 90 degrees
	double angleOffsetPercent = (currentAngle - targetAngle) / 10;

	// Limit angle offset to range of -10 to 10 (-100% to 100%)
	if (angleOffsetPercent > 1)
		angleOffsetPercent = 1;
	if (angleOffsetPercent < -1)
		angleOffsetPercent = -1;

	// Scale the speed based on distance to target
	double speedMultiplier = 1;

	// Back up if passed target
	if (currentDistance > targetDistance)
	{
		speedMultiplier = -1;
	}

	// Set speed offset to the speed multiplied by the percent that the robot is to 90 degrees off target
	double leftSpeedMult = 1.0 - angleOffsetPercent * speedMultiplier;
	double rightSpeedMult = 1.0 + angleOffsetPercent * speedMultiplier;

	// If robot should stop after reaching target
	if (stopAtTarget)
	{
		// Distance to decelerate is greater for higher speeds
		double decelerateDistance = 40 * (fabs(leftSpeed) + fabs(rightSpeed)) / 2;

		// Slow down on approach
		if (fabs(targetDistance - currentDistance) < decelerateDistance)
			speedMultiplier *= fabs(targetDistance - currentDistance) / decelerateDistance;

		// Prevent robot from driving too slowly
		if (fabs(speedMultiplier) < 0.5)
			speedMultiplier *= 0.5 / fabs(speedMultiplier);
	}

	driveBase->Drive(leftSpeed * speedMultiplier * leftSpeedMult, rightSpeed * speedMultiplier * rightSpeedMult);

	// Returns true if distance to target is less than 3 (0.5 foot range)
	return abs( currentDistance - targetDistance ) < 3;
}

bool AutoController::AutoRotateToAngle( double leftSpeed, double rightSpeed, double targetAngle, bool stopAtTarget )
{
	// If left and right speeds are equal, assume directions are meant to be opposite
	if (leftSpeed == rightSpeed)
		rightSpeed *= -1;

	// If speeds would cause robot to rotate counter-clockwise, invert them because the sign of speeds should rotate clockwise
	if (rightSpeed > leftSpeed)
	{
		rightSpeed *= -1;
		leftSpeed *= -1;
	}

	// Get sensor data
	double targetAngleOffset = targetAngle - gyroscope->GetAngle();

	double speedMult = 1;

	if (fabs(targetAngleOffset) < 15)
		speedMult = fabs(targetAngleOffset) / 15.0;

	// Turn clockwise
	if ( targetAngleOffset > 4 )
		driveBase->Drive(leftSpeed * speedMult, rightSpeed * speedMult);

	// Turn counter-clockwise
	else if ( targetAngleOffset < -4 )
		driveBase->Drive(-leftSpeed * speedMult, -rightSpeed * speedMult);

	// Stop
	else if (stopAtTarget)
		driveBase->Stop();


	// Return true if angle is within range of (targetAngle - 4, targetAngle + 4))
	return abs( targetAngleOffset ) < 4;
}
