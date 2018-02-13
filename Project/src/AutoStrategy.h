#ifndef AUTOSTRATEGY_H
#define AUTOSTRATEGY_H

#include <WPILib.h>
#include "DriveBase.h"

class AutoStrategy
{
public:
	enum InstructionType { WAIT_UNTIL, WAIT_TIME, DRIVE_TO, DRIVE_DIST, ROTATE_TO, ROTATE_DEG, RESET_DIST_0, RESET_DIST_ULTRA, RESET_ANGLE  };

	struct Instruction
	{
		InstructionType type;
		double target; // Distance or angle, depending on instruction
		double speed;
	};

	AutoStrategy(Instruction* instructions, unsigned int instructionCount);
	~AutoStrategy();

	// Name:	Execute
	// Parameters:
	//		driveBase (I/O)	- The drivebase of the robot. Used to read encoders and set motor speeds.
	//		gyroscope (I)	- The gyroscope on the robot. Used to detect robot's rotation.
	//		autoTimer (I)	- Timer that measures how much time has passed since the match started. Used for timing.
	// Return:
	//		True if strategy is complete.
	bool Execute( DriveBase& driveBase, ADXRS450_Gyro& gyroscope, Timer& autoTimer );

private:
	unsigned int instructionCount;
	Instruction* instructions;
	unsigned int currentInstruction;
	double instructionStartTime;
	double instructionStartDistance;
	double instructionStartAngle;

	bool AutoDriveToDist( double speed, double distance, double angle, double targetDistance, double targetAngle, DriveBase& driveBase );
	bool AutoRotateToAngle( double speed, double angle, double targetAngle, DriveBase& driveBase );

};

#endif
