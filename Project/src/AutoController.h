#ifndef AUTONOMOUS_H
#define AUTONOMOUS_H

#include <WPILib.h>
#include "DriveBase.h"

class AutoController
{
public:
	enum InstructionType { WAIT_UNTIL, WAIT_TIME, DRIVE_TO, DRIVE_DIST, ROTATE_TO, ROTATE_DEG, RESET_DIST_0, RESET_DIST_ULTRA, DRIBBLE  };

	struct Instruction
	{
		InstructionType type;
		double target; // Distance or angle, depending on instruction
		double speed;
		double xAxis;
		double yAxis;
		double points;
		double pointsAngle;
	};

	struct InstructionSet
	{
		const Instruction* steps;
		unsigned int count;
	};

	AutoController(DriveBase* driveBase, ADXRS450_Gyro* gyroscope);
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
	ADXRS450_Gyro* gyroscope;

	Timer timer;

	InstructionSet instructionSet;

	unsigned int currentInstruction;
	double instructionStartTime;
	double instructionStartDistance;
	double instructionStartAngle;

	bool AutoDriveToDist( double speed, double targetDistance, double targetAngle );
	bool AutoRotateToAngle( double speed, double targetAngle );
	bool AutoDribble( int xAxis, int yAxis, int points, int speed, int pointsAngle );

	double hypotneuse;
	double pointsReached;
	double angle;

};

#endif
