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
private:
	//cs::CvSink cvSink; // OpenCV image data sink
	grip::GripPipeline gripPipeline;

	Vector2 targetPositionRelative;
	bool targetIsVisible;

public:
	CameraTracking(int imgWidth, int imgHeight, int imgExposure);
	~CameraTracking();

	static void VisionThread();
	void UpdateVision();

	Vector2 GetTargetPositionRelative();
	double GetRobotPosition(float position);
	float CalculateAngle(float x, float y);

};

#endif
