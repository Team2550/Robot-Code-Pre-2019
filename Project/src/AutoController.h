#ifndef AUTONOMOUS_H
#define AUTONOMOUS_H

#include <WPILib.h>
#include <iostream>
#include "DriveBase.h"
#include "Bulldozer.h"

class AutoController
{
public:
	enum InstructionType { WAIT_UNTIL, WAIT_TIME, DRIVE_TO, DRIVE_DIST, ROTATE_TO, ROTATE_DEG, RESET_DIST_0, EXTEND, RETRACT }; //, RESET_DIST_ULTRA  };

	struct Instruction
	{
		InstructionType type;
		double target; // Distance or angle, depending on instruction
		double leftSpeed; // Acts as speed of both sides if only one specified.
		double rightSpeed = leftSpeed; // Default to both sides sharing the same speed
	};

	struct InstructionSet
	{
		const Instruction* steps;
		unsigned int count;
	};

	struct PositionOptions
	{
		const InstructionSet* leftOption;
		const InstructionSet* rightOption;
	};

	AutoController(DriveBase* driveBase, Bulldozer* bulldozer, Gyro* gyroscope);
	~AutoController();

	// Name:	Execute
	// Parameters:
	//		driveBase (I/O)	- The drivebase of the robot. Used to read encoders and set motor speeds.
	//		gyroscope (I)	- The gyroscope on the robot. Used to detect robot's rotation.
	//		autoTimer (I)	- Timer that measures how much time has passed since the match started. Used for timing.
	// Return:
	//		True if strategy is complete.
	void Init(const InstructionSet instructionSet);
	bool Execute();

private:
	DriveBase* driveBase;
	Bulldozer* bulldozer;
	Gyro* gyroscope;

	Timer timer;

	InstructionSet instructionSet;

	unsigned int currentInstruction;
	double instructionStartTime;
	double instructionStartDistance;
	double instructionTargetAngle;
	bool bulldozerExtended;

	bool AutoDriveToDist( double leftSpeed, double rightSpeed, double targetDistance, double targetAngle );
	bool AutoRotateToAngle( double leftSpeed, double rightSpeed, double targetAngle );

};

#endif
