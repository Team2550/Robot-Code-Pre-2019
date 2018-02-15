#ifndef CAMERA_TRACKING_H
#define CAMERA_TRACKING_H

#include <WPILib.h>
#include <math.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include "GripPipeline.h"

struct Vector2
{
	float x;
	float y;
};

class CameraTracking
{
public:
	CameraTracking(int imgWidth, int imgHeight, int imgExposure);
	~CameraTracking();

	void LaunchVisionThread();

	Vector2 GetTargetPositionRelative();
	double GetRobotPosition(float position);
	float CalculateAngle(float x, float y);

private:
	static int imgWidth;
	static int imgHeight;
	static int imgExposure;

	static Vector2 targetPositionRelative;
	static bool targetIsVisible;

	static void VisionThread();
};

#endif
