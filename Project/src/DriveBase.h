#ifndef DRIVEBASE_H
#define DRIVEBASE_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"

const double DEFAULT_AUTODRIVE_THRESHOLD_INCHES = 5;

class DriveBase
{
public:
	typedef void (*DriveActionCallback) (bool); // Bool parameter will be true if action was completed.

	DriveBase(int leftMotorPort, int rightMotorPort,
			  int leftEncoderPortA, int leftEncoderPortB,
			  int rightEncoderPortA, int rightEncoderPortB,
			  double encoderDistancePerPulse);

	double GetLeftSpeed();
	double GetRightSpeed();
	void Drive(double leftSpeed, double rightSpeed);
	void Drive(double speed);
	void Stop();
	bool IsStopped();
	void ResetDistance();
	double GetLeftDistance();
	double GetRightDistance();
	void SetTrim(float leftTrim, float rightTrim);

	//================== Auto Drive ==================
	void UpdateAutoDrive();

	void AutoDriveTo(double left,
			double right, double thresholdInches = DEFAULT_AUTODRIVE_THRESHOLD_INCHES, DriveActionCallback callback = nullptr );

	void AutoDriveDist(double distance,
			double thresholdInches = DEFAULT_AUTODRIVE_THRESHOLD_INCHES, DriveActionCallback callback = nullptr );

	void AutoRotate(double degrees,
			double thresholdInches = DEFAULT_AUTODRIVE_THRESHOLD_INCHES, DriveActionCallback callback = nullptr );

private:
	Victor leftMotor;
	Victor rightMotor;

	Encoder leftEncoder;
	Encoder rightEncoder;

	float leftTrim;
	float rightTrim;

	double leftTargetDistance;
	double rightTargetDistance;
	double threshold;

	DriveActionCallback nextCallback;

	void RunCallback(bool actionCompleted);
	void SetNextCallback(DriveActionCallback func);

};

#endif
